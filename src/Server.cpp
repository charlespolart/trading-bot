#include "Server.hpp"

Server::Server()
{
    this->_api = new binapi::rest::api(
        this->_ioctx, "api.binance.com", "443", "", "", 10000);
    this->_ws = new binapi::ws::websockets_pool(
        this->_ioctx, "stream.binance.com", "9443");
}

Server::~Server()
{
    if (this->_api)
        delete this->_api;
    if (this->_ws)
        delete this->_ws;
    for (size_t i = 0; i < this->_coins.size(); ++i)
    {
        if (this->_coins[i])
            delete this->_coins[i];
    }
}

/* PRIVATE */

int Server::fetchCoins()
{
    std::cout << "Fetching coins..." << std::endl;

    std::vector<std::string> pairs;

    if (FetchPairs::fetch(pairs) == EXIT_FAILURE)
        return (EXIT_FAILURE);

    auto exchange_info = this->_api->exchange_info();

    if (!exchange_info)
    {
        std::cerr << exchange_info.errmsg << std::endl;
        return (EXIT_FAILURE);
    }
    for (auto it = exchange_info.v.symbols.begin(); it != exchange_info.v.symbols.end(); ++it)
    {
        if (it->second.quoteAsset == "BTC" && it->second.status == "TRADING")
        {
            std::string pair = it->first;
            binapi::double_type stepSize = it->second.get_filter_lot().stepSize;
            // if (it->first == "ETHBTC" || it->first == "DOGEBTC" || it->first == "ADABTC" || it->first == "MATICBTC" || it->first == "BNBBTC" || it->first == "XRPBTC" || it->first == "AXSBTC" || it->first == "DOTBTC" || it->first == "LINKBTC" || it->first == "THETABTC")
            // if (it->first == "MATICBTC")
            if (std::find(pairs.begin(), pairs.end(), it->first) != pairs.end())
                this->_coins.emplace_back(new Coin(this->_ioctx, this->_api, this->_ws, pair, stepSize, this->_users));
        }
    }
    return (EXIT_SUCCESS);
}

void Server::contextRun_thread()
{
    while (1)
    {
        this->_ioctx.run();
        this->_ioctx.restart();
        std::cout << "ioctx restart" << std::endl;
        Tools::Log::writeToFile("ioctx", "ioctx restart");
    }
}

bool Server::streamToKline(pqxx::stream_from &stream, binapi::ws::kline_t &kline)
{
    std::tuple<std::string, size_t, size_t, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, int, std::string> row;

    if (!(stream >> row))
        return (false);
    std::string interval = std::get<0>(row);
    size_t openTime = std::get<1>(row);
    size_t closeTime = std::get<2>(row);
    std::string openPrice = std::get<3>(row);
    std::string closePrice = std::get<4>(row);
    std::string lowPrice = std::get<5>(row);
    std::string highPrice = std::get<6>(row);
    std::string volume = std::get<7>(row);
    std::string quoteVolume = std::get<8>(row);
    std::string takerBuyVolume = std::get<9>(row);
    std::string takerBuyQuoteVolume = std::get<10>(row);
    int tradeCount = std::get<11>(row);
    std::string pair = std::get<12>(row);

    kline.t = openTime;
    kline.T = closeTime;
    kline.s = pair;
    kline.i = interval;
    kline.o = std::stod(openPrice);
    kline.c = std::stod(closePrice);
    kline.h = std::stod(highPrice);
    kline.l = std::stod(lowPrice);
    kline.v = std::stod(volume);
    kline.n = tradeCount;
    kline.q = std::stod(quoteVolume);
    kline.V = std::stod(takerBuyVolume);
    kline.Q = std::stod(takerBuyQuoteVolume);
    return (true);
}

int Server::runHistory()
{
    Database database(std::getenv("DATABASE_HISTORY_NAME"));

    if (!database.getConnection().is_open())
        return (EXIT_FAILURE);

    try
    {
        Database databaseResult(std::getenv("DATABASE_RESULT_HISTORY_NAME"));
        if (!databaseResult.getConnection().is_open())
            return (EXIT_FAILURE);
        pqxx::transaction transactionResult(databaseResult.getConnection());
        transactionResult.exec("DELETE FROM transactions");
        transactionResult.commit();
        databaseResult.getConnection().close();

        pqxx::transaction transaction(database.getConnection());
        for (size_t i = 0; i < this->_coins.size(); ++i)
        {
            pqxx::result result = transaction.exec("SELECT MIN(open_time) FROM candles WHERE pair='" + this->_coins[i]->_pair + "'");
            if (result.empty())
                return (EXIT_FAILURE);
            size_t endTime = 0;
            try
            {
                endTime = std::stoul(result.front().front().c_str());
            }
            catch (const std::exception &)
            {
                continue;
            }
            if (this->_coins[i]->init(endTime) == EXIT_FAILURE)
                continue;

            pqxx::stream_from stream = pqxx::stream_from::query(transaction, "SELECT interval, open_time, close_time, open_price, close_price, low_price, high_price, volume, quote_volume, taker_buy_volume, taker_buy_quote_volume, trade_count, pair FROM candles WHERE pair='" + this->_coins[i]->_pair + "' AND (interval='" + DOWN1_INTERVAL + "' OR interval='" + MAIN_INTERVAL + "' OR interval='" + UP1_INTERVAL + "' OR interval='" + UP2_INTERVAL + "' OR interval='" + UP3_INTERVAL + "') ORDER BY open_time ASC");
            binapi::ws::kline_t kline;
            while (this->streamToKline(stream, kline))
            {
                this->_coins[i]->update_callback(kline);
                // std::cout << this->_coins[i]->_pair << " " << std::get<1>(row) << std::endl;
            }
        }
        database.getConnection().close();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (EXIT_SUCCESS);
}

int Server::runProduction()
{
    std::thread contextThread;

    for (size_t i = 0; i < this->_coins.size(); ++i)
    {
        if (this->_coins[i]->init() == EXIT_FAILURE)
            return (EXIT_FAILURE);
        if (!contextThread.joinable())
            contextThread = std::thread(&Server::contextRun_thread, this);
    }
    if (contextThread.joinable())
        contextThread.join();
    std::cout << "Thread ended" << std::endl;
    return (EXIT_SUCCESS);
}

/* PUBLIC */

int Server::run()
{
    if (!HISTORY && FetchUsers::fetch(this->_ioctx, this->_users) == EXIT_FAILURE)
        return (EXIT_FAILURE);
    if (this->fetchCoins() == EXIT_FAILURE)
        return (EXIT_FAILURE);
    HISTORY ? this->runHistory() : this->runProduction();
    return (EXIT_SUCCESS);
}

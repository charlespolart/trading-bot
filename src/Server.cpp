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
            if (it->first == "ETHBTC" || it->first == "XRPBTC" || it->first == "BNBBTC" || it->first == "LTCBTC" || it->first == "VETBTC" || it->first == "DOTBTC" || it->first == "ADABTC" || it->first == "NANOBTC" || it->first == "TXRBTC" || it->first == "LINKBTC" || it->first == "BCHBTC")
                this->_coins.emplace_back(new Coin(pair, stepSize));
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
    }
}

/* PUBLIC */

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
            if (this->_coins[i]->init(this->_api, endTime) == EXIT_FAILURE)
                continue;

            pqxx::stream_from stream = pqxx::stream_from::query(transaction, "SELECT interval, open_time, close_time, open_price, close_price, low_price, high_price, volume, quote_volume, taker_buy_volume, taker_buy_quote_volume, trade_count FROM candles WHERE pair='" + this->_coins[i]->_pair + "' ORDER BY open_time ASC");
            std::tuple<std::string, size_t, size_t, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, int> row;
            while (stream >> row)
            {
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
                binapi::ws::kline_t kline;

                kline.t = openTime;
                kline.T = closeTime;
                kline.s = this->_coins[i]->_pair;
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
                this->_coins[i]->updateCallback(kline);
                //std::cout << this->_coins[i]->_pair << " " << std::get<1>(row) << std::endl;
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
    std::thread contextThread = std::thread(&Server::contextRun_thread, this);

    for (size_t i = 0; i < this->_coins.size(); ++i)
    {
        if (this->_coins[i]->init(this->_api) == EXIT_FAILURE)
            return (EXIT_FAILURE);

        Coin *coin = this->_coins[i];
        this->_ws->klines(this->_coins[i]->_pair.c_str(), INTERVAL, [this, coin](const char *fl, int ec, std::string errmsg, binapi::ws::kline_t kline) {
            if (ec)
            {
                std::cerr << "subscribe klines error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                return (false);
            }
            if (this->_currentKlines.find(coin->_pair) != this->_currentKlines.end() &&
                kline.t != this->_currentKlines[coin->_pair].t)
            {
                coin->updateCallback(this->_currentKlines[coin->_pair]);
            }
            this->_currentKlines[coin->_pair] = kline;
            return (true);
        });
    }
    if (contextThread.joinable())
        contextThread.join();
    return (EXIT_SUCCESS);
}

int Server::run()
{
    if (EMA_SHORT >= EMA_LONG || std::min(EMA_SHORT, EMA_LONG) <= 0)
    {
        std::cerr << "SHORT must be smaller than LONG (Range [1-1000])" << std::endl;
        return (EXIT_FAILURE);
    }
    if (!HISTORY && FetchUsers::fetch(this->_ioctx, this->_users))
        return (EXIT_FAILURE);
    if (this->fetchCoins() == EXIT_FAILURE)
        return (EXIT_FAILURE);
    HISTORY ? this->runHistory() : this->runProduction();
    return (EXIT_SUCCESS);
}

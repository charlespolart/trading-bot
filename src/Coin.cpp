#include "Coin.hpp"

Coin::Coin(const std::string &pair, binapi::double_type stepSize) : _pair(pair),
                                                                    _stepSize(stepSize)
{
}

Coin::~Coin()
{
}

/* PUBLIC */

binapi::rest::new_order_info_full_t Coin::getBuyOrderInfo() const
{
    return (this->_buyOrderInfo);
}

int Coin::init(binapi::rest::api *api, size_t endTime)
{
    std::cout << "Starting " << this->_pair << std::endl;

    auto klines = api->klines(this->_pair, INTERVAL, 1000, 0, endTime);

    if (!klines)
    {
        std::cerr << klines.errmsg << std::endl;
        return (EXIT_FAILURE);
    }
    if (klines.v.klines.size() <= 1)
    {
        std::cerr << this->_pair << ": Unable to get klines" << std::endl;
        return (EXIT_FAILURE);
    }
    klines.v.klines.pop_back();
    this->_indicators.init(klines.v.klines);
    return (EXIT_SUCCESS);
}

void Coin::updateCallback(const binapi::ws::kline_t &kline)
{
    signal_e signal = signal_e::NONE;

    this->_indicators.update(kline);
    signal = this->_indicators.fetchSignal();
    if (signal == signal_e::BUY)
    {
        this->_buyOrderInfo.status = "FILLED";
        this->_buyOrderInfo.price = kline.c;
        Database database(HISTORY ? std::getenv("DATABASE_RESULT_HISTORY_NAME") : std::getenv("DATABASE_RESULT_NAME"));
        pqxx::work work(database.getConnection());
        work.exec("INSERT INTO transactions VALUES ('" + kline.s + "', '" + kline.i + "', '" + std::to_string(kline.t) + "', '" + std::to_string(kline.T) + "', '" + Tools::Convert::to_string(kline.o, 8) + "', '" + Tools::Convert::to_string(kline.c, 8) + "', '" + Tools::Convert::to_string(kline.l, 8) + "', '" + Tools::Convert::to_string(kline.h, 8) + "', '" + Tools::Convert::to_string(kline.v, 8) + "', '" + Tools::Convert::to_string(kline.q, 8) + "', '" + Tools::Convert::to_string(kline.V, 8) + "', '" + Tools::Convert::to_string(kline.Q, 8) + "', '" + std::to_string(kline.n) + "', 'BUY', '" + Tools::Convert::to_string(this->_indicators.getEMACrossStatus().EMAShort.getStatus(), 8) + "',  '" + Tools::Convert::to_string(this->_indicators.getRSIStatus(), 8) + "')");
        work.commit();
        database.getConnection().close();
        //std::cout << "INSERT INTO transactions VALUES ('" + kline.s + "', '" + kline.i + "', '" + std::to_string(kline.t) + "', '" + std::to_string(kline.T) + "', '" + Tools::Convert::to_string(kline.o, 8) + "', '" + Tools::Convert::to_string(kline.c, 8) + "', '" + Tools::Convert::to_string(kline.l, 8) + "', '" + Tools::Convert::to_string(kline.h, 8) + "', '" + Tools::Convert::to_string(kline.v, 8) + "', '" + Tools::Convert::to_string(kline.q, 8) + "', '" + Tools::Convert::to_string(kline.V, 8) + "', '" + Tools::Convert::to_string(kline.Q, 8) + "', '" + std::to_string(kline.n) + "', 'BUY', '" + Tools::Convert::to_string(this->_indicators.getEMACrossStatus().EMAShort.getStatus(), 8) + "',  '" + Tools::Convert::to_string(this->_indicators.getRSIStatus(), 8) + "')" << std::endl;
    }
    else if (signal == signal_e::SELL)
    {
        this->_buyOrderInfo = binapi::rest::new_order_info_full_t{};
        Database database(HISTORY ? std::getenv("DATABASE_RESULT_HISTORY_NAME") : std::getenv("DATABASE_RESULT_NAME"));
        pqxx::work work(database.getConnection());
        work.exec("INSERT INTO transactions VALUES ('" + kline.s + "', '" + kline.i + "', '" + std::to_string(kline.t) + "', '" + std::to_string(kline.T) + "', '" + Tools::Convert::to_string(kline.o, 8) + "', '" + Tools::Convert::to_string(kline.c, 8) + "', '" + Tools::Convert::to_string(kline.l, 8) + "', '" + Tools::Convert::to_string(kline.h, 8) + "', '" + Tools::Convert::to_string(kline.v, 8) + "', '" + Tools::Convert::to_string(kline.q, 8) + "', '" + Tools::Convert::to_string(kline.V, 8) + "', '" + Tools::Convert::to_string(kline.Q, 8) + "', '" + std::to_string(kline.n) + "', 'SELL', '" + Tools::Convert::to_string(this->_indicators.getEMACrossStatus().EMAShort.getStatus(), 8) + "',  '" + Tools::Convert::to_string(this->_indicators.getRSIStatus(), 8) + "')");
        work.commit();
        database.getConnection().close();
        //std::cout << "INSERT INTO transactions VALUES ('" + kline.s + "', '" + kline.i + "', '" + std::to_string(kline.t) + "', '" + std::to_string(kline.T) + "', '" + Tools::Convert::to_string(kline.o, 8) + "', '" + Tools::Convert::to_string(kline.c, 8) + "', '" + Tools::Convert::to_string(kline.l, 8) + "', '" + Tools::Convert::to_string(kline.h, 8) + "', '" + Tools::Convert::to_string(kline.v, 8) + "', '" + Tools::Convert::to_string(kline.q, 8) + "', '" + Tools::Convert::to_string(kline.V, 8) + "', '" + Tools::Convert::to_string(kline.Q, 8) + "', '" + std::to_string(kline.n) + "', 'SELL', '" + Tools::Convert::to_string(this->_indicators.getEMACrossStatus().EMAShort.getStatus(), 8) + "',  '" + Tools::Convert::to_string(this->_indicators.getRSIStatus(), 8) + "')" << std::endl;
    }
}

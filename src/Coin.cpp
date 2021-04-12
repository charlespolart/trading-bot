#include "Coin.hpp"

Coin::Coin(const std::string &pair, binapi::double_type stepSize) : _pair(pair),
                                                                    _stepSize(stepSize)
{
}

Coin::~Coin()
{
}

/* PUBLIC */

void Coin::updateCallback(const binapi::ws::kline_t &kline)
{
    result_t result = this->_EMACross.update(kline);

    /* Buy */
    if (result.cross == cross_e::CROSS_UP)
    {
        Database database(HISTORY ? std::getenv("DATABASE_RESULT_HISTORY_NAME") : std::getenv("DATABASE_RESULT_NAME"));
        pqxx::work work(database.getConnection());
        work.exec("INSERT INTO transaction VALUES ('" + kline.s + "', '" + kline.i + "', '" + std::to_string(kline.t) + "', '" + std::to_string(kline.T) + "', '" + Tools::Convert::to_string(kline.o, 8) + "', '" + Tools::Convert::to_string(kline.c, 8) + "', '" + Tools::Convert::to_string(kline.l, 8) + "', '" + Tools::Convert::to_string(kline.h, 8) + "', '" + Tools::Convert::to_string(kline.v, 8) + "', '" + Tools::Convert::to_string(kline.q, 8) + "', '" + Tools::Convert::to_string(kline.V, 8) + "', '" + Tools::Convert::to_string(kline.Q, 8) + "', '" + std::to_string(kline.n) + "', 'BUY', '" + Tools::Convert::to_string(result.EMA.currentShort, 8) + "')");
        work.commit();
        database.getConnection().close();
        //std::cout << "INSERT INTO transaction VALUES ('" + kline.s + "', '" + kline.i + "', '" + std::to_string(kline.t) + "', '" + std::to_string(kline.T) + "', '" + Tools::Convert::to_string(kline.o, 8) + "', '" + Tools::Convert::to_string(kline.c, 8) + "', '" + Tools::Convert::to_string(kline.l, 8) + "', '" + Tools::Convert::to_string(kline.h, 8) + "', '" + Tools::Convert::to_string(kline.v, 8) + "', '" + Tools::Convert::to_string(kline.q, 8) + "', '" + Tools::Convert::to_string(kline.V, 8) + "', '" + Tools::Convert::to_string(kline.Q, 8) + "', '" + std::to_string(kline.n) + "', 'BUY', '" + Tools::Convert::to_string(result.EMA.currentShort, 8) + "')" << std::endl;
    }

    /* Sell */
    if (result.cross == cross_e::CROSS_DOWN)
    {
        Database database(HISTORY ? std::getenv("DATABASE_RESULT_HISTORY_NAME") : std::getenv("DATABASE_RESULT_NAME"));
        pqxx::work work(database.getConnection());
        work.exec("INSERT INTO transaction VALUES ('" + kline.s + "', '" + kline.i + "', '" + std::to_string(kline.t) + "', '" + std::to_string(kline.T) + "', '" + Tools::Convert::to_string(kline.o, 8) + "', '" + Tools::Convert::to_string(kline.c, 8) + "', '" + Tools::Convert::to_string(kline.l, 8) + "', '" + Tools::Convert::to_string(kline.h, 8) + "', '" + Tools::Convert::to_string(kline.v, 8) + "', '" + Tools::Convert::to_string(kline.q, 8) + "', '" + Tools::Convert::to_string(kline.V, 8) + "', '" + Tools::Convert::to_string(kline.Q, 8) + "', '" + std::to_string(kline.n) + "', 'SELL', '" + Tools::Convert::to_string(result.EMA.currentShort, 8) + "')");
        work.commit();
        database.getConnection().close();
        //std::cout << "INSERT INTO transaction VALUES ('" + kline.s + "', '" + kline.i + "', '" + std::to_string(kline.t) + "', '" + std::to_string(kline.T) + "', '" + Tools::Convert::to_string(kline.o, 8) + "', '" + Tools::Convert::to_string(kline.c, 8) + "', '" + Tools::Convert::to_string(kline.l, 8) + "', '" + Tools::Convert::to_string(kline.h, 8) + "', '" + Tools::Convert::to_string(kline.v, 8) + "', '" + Tools::Convert::to_string(kline.q, 8) + "', '" + Tools::Convert::to_string(kline.V, 8) + "', '" + Tools::Convert::to_string(kline.Q, 8) + "', '" + std::to_string(kline.n) + "', 'SELL', '" + Tools::Convert::to_string(result.EMA.currentShort, 8) + "')" << std::endl;
    }
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
    if (this->_EMACross.init(klines.v.klines) == EXIT_FAILURE)
    {
        std::cerr << this->_pair << ": Init error" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

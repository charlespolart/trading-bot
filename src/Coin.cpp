#include "Coin.hpp"

Coin::Coin(boost::asio::io_context &ioctx, const std::string &pair, binapi::double_type stepSize, const std::vector<User *> &users) : _pair(pair),
                                                                                                                                      _ioctx(ioctx),
                                                                                                                                      _stepSize(stepSize),
                                                                                                                                      _users(users)
{
}

Coin::~Coin()
{
}

/* PUBLIC */

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
    this->_indicators.update(kline);

    signal_e signal = this->fetchSignal();

    /* Buy signal */
    if (!this->_status.bought && signal == signal_e::BUY)
    {
        this->buy(kline);
    }
    /* Sell signal */
    else if (this->_status.bought && signal == signal_e::SELL)
    {
        this->sell(kline);
    }
}

/* PRIVATE */

signal_e Coin::fetchSignal()
{
    statusEMACross_t statusEMACrossBuy = this->_indicators.getEMACrossBuyStatus();
    binapi::double_type RSI = this->_indicators.getRSIStatus();

    if (statusEMACrossBuy.cross == cross_e::CROSS_UP && RSI >= RSI_MIN_BUY)
    {
        return (signal_e::BUY);
    }
    else if (statusEMACrossBuy.cross == cross_e::CROSS_DOWN)
    {
        return (signal_e::SELL);
    }
    return (signal_e::NONE);
}

void Coin::buy(const binapi::ws::kline_t &kline)
{
    this->_status.bought = true;
    for (size_t i = 0; i < this->_users.size(); ++i)
    {
        User *user = this->_users[i];
        boost::asio::post(this->_ioctx, [this, user] {
            user->buy(this->_pair);
        });
    }
    this->writeTransaction("buy", kline);
}

void Coin::sell(const binapi::ws::kline_t &kline)
{
    this->_status.bought = false;
    for (size_t i = 0; i < this->_users.size(); ++i)
    {
        User *user = this->_users[i];
        boost::asio::post(this->_ioctx, [this, user] {
            user->sell(this->_pair, this->_stepSize);
        });
    }
    this->writeTransaction("sell", kline);
}

void Coin::setStopLoss(binapi::double_type value)
{
    this->_status.currentStopLoss = value;
}

void Coin::checkStopLossStatus(const binapi::ws::kline_t &kline)
{
    if (HISTORY)
    {
        if (kline.l <= this->_status.currentStopLoss)
        {
            this->_status.bought = false;
            binapi::ws::kline_t klineHistory = kline;
            klineHistory.c = this->_status.currentStopLoss;
            this->writeTransaction("sell", klineHistory);
        }
    }
    else
    {
        //if triggered
        //this->_status.bought = false;
        //database.writeTransaction("sell", kline);
    }
}

void Coin::writeTransaction(const std::string &type, const binapi::ws::kline_t &kline)
{
    Database database(HISTORY ? std::getenv("DATABASE_RESULT_HISTORY_NAME") : std::getenv("DATABASE_RESULT_NAME"));
    database.writeTransaction(type, kline);
    database.getConnection().close();
}

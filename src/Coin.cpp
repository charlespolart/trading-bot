#include "Coin.hpp"

Coin::Coin(boost::asio::io_context &ioctx, binapi::rest::api *api, binapi::ws::websockets_pool *ws, const std::string &pair, binapi::double_type stepSize, const std::vector<User *> &users) : _pair(pair),
                                                                                                                                                                                               _ioctx(ioctx),
                                                                                                                                                                                               _api(api),
                                                                                                                                                                                               _ws(ws),
                                                                                                                                                                                               _stepSize(stepSize),
                                                                                                                                                                                               _users(users)
{
}

Coin::~Coin()
{
}

/* PUBLIC */

int Coin::init(size_t endTime)
{
    std::cout << "Starting " << this->_pair << std::endl;

    this->_indicators[DOWN1_INTERVAL] = Indicators(this->_api, this->_ws, this->_pair, DOWN1_INTERVAL);
    this->_indicators[MAIN_INTERVAL] = Indicators(this->_api, this->_ws, this->_pair, MAIN_INTERVAL);
    this->_indicators[UP1_INTERVAL] = Indicators(this->_api, this->_ws, this->_pair, UP1_INTERVAL);
    this->_indicators[UP2_INTERVAL] = Indicators(this->_api, this->_ws, this->_pair, UP2_INTERVAL);
    this->_indicators[UP3_INTERVAL] = Indicators(this->_api, this->_ws, this->_pair, UP3_INTERVAL);
    for (std::map<std::string, Indicators>::iterator it = this->_indicators.begin(); it != this->_indicators.end(); ++it)
    {
        if (it->second.init([this](const binapi::ws::kline_t &kline)
                            { this->update_callback(kline); },
                            endTime) == EXIT_FAILURE)
            return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

void Coin::update_callback(const binapi::ws::kline_t &kline)
{
    this->_indicators[kline.i].update(kline);
    if (kline.i == MAIN_INTERVAL)
        this->trade(kline);
}

/* PRIVATE */

signal_e Coin::fetchSignal()
{
    statusEMACross_t statusEMACross = this->_indicators[MAIN_INTERVAL].getEMACross().getStatus();
    binapi::double_type RSIDown1 = this->_indicators[DOWN1_INTERVAL].getRSI().getStatus();
    binapi::double_type RSIMain = this->_indicators[MAIN_INTERVAL].getRSI().getStatus();
    binapi::double_type RSIUp1 = this->_indicators[UP1_INTERVAL].getRSI().getStatus();
    binapi::double_type RSIUp2 = this->_indicators[UP2_INTERVAL].getRSI().getStatus();
    binapi::double_type RSIUp3 = this->_indicators[UP3_INTERVAL].getRSI().getStatus();
    bool down1Crossed = this->_indicators[DOWN1_INTERVAL].getEMACross().crossed();
    bool up1Crossed = this->_indicators[UP1_INTERVAL].getEMACross().crossed();
    bool up2Crossed = this->_indicators[UP2_INTERVAL].getEMACross().crossed();
    bool up3Crossed = this->_indicators[UP3_INTERVAL].getEMACross().crossed();

    if (statusEMACross.cross == cross_e::CROSS_UP &&
        RSIDown1 >= RSI_MIN_BUY && RSIMain >= RSI_MIN_BUY && RSIUp1 >= RSI_MIN_BUY && RSIUp2 >= RSI_MIN_BUY && RSIUp3 >= RSI_MIN_BUY &&
        down1Crossed && !up1Crossed && !up2Crossed && !up3Crossed)
    {
        this->_status.crossed = true;
    }
    else if (this->_status.crossed && !this->_status.bought && RSIMain <= 60)
    {
        return (signal_e::BUY);
    }
    else if (/*kline.c <= this->_status.currentStopLoss || */ /*kline.c <= statusEMACross.EMALong.getStatus() || */ this->_status.crossed && statusEMACross.cross == cross_e::CROSS_DOWN)
    {
        this->_status.crossed = false;
        return (signal_e::SELL);
    }
    return (signal_e::NONE);
}

void Coin::trade(const binapi::ws::kline_t &kline)
{
    signal_e signal = this->fetchSignal();

    /* Buy signal */
    if (signal == signal_e::BUY)
        this->buy(kline);
    /* Sell signal */
    else if (signal == signal_e::SELL)
        this->sell(kline);
    /*if (this->_status.bought)
        this->updateStopLoss();*/
}

void Coin::buy(const binapi::ws::kline_t &kline)
{
    if (!this->_status.bought)
    {
        this->_status.bought = true;
        for (size_t i = 0; i < this->_users.size(); ++i)
        {
            User *user = this->_users[i];
            boost::asio::post(this->_ioctx, [this, user]
                              { user->buy(this->_pair); });
        }
        this->writeTransaction("buy", kline);
    }
}

void Coin::sell(const binapi::ws::kline_t &kline)
{
    if (this->_status.bought)
    {
        this->_status = status_t{};
        for (size_t i = 0; i < this->_users.size(); ++i)
        {
            User *user = this->_users[i];
            boost::asio::post(this->_ioctx, [this, user]
                              { user->sell(this->_pair, this->_stepSize); });
        }
        this->writeTransaction("sell", kline);
    }
}

/*void Coin::updateStopLoss()
{
    binapi::double_type ATR = this->_indicators[MAIN_INTERVAL].getATRStatus();
    binapi::double_type swingLow = this->_indicators[MAIN_INTERVAL].getSwingLowStatus();
    binapi::double_type stopLoss = swingLow - ATR * 2.0;

    if (stopLoss > this->_status.currentStopLoss)
        this->_status.currentStopLoss = stopLoss;
}*/

void Coin::writeTransaction(const std::string &type, const binapi::ws::kline_t &kline)
{
    Database database(HISTORY ? std::getenv("DATABASE_RESULT_HISTORY_NAME") : std::getenv("DATABASE_RESULT_NAME"));
    database.writeTransaction(type, kline);
    database.getConnection().close();
}

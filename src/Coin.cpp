#include "Coin.hpp"

Coin::Coin(const std::string &pair, binapi::double_type stepSize) : _pair(pair),
                                                                    _stepSize(stepSize)
{
}

Coin::~Coin()
{
}

/* PUBLIC */

void Coin::init(binapi::rest::api *api, binapi::ws::websockets_pool *ws)
{
    std::cout << "Starting " << this->_pair << std::endl;

    auto klines = api->klines(this->_pair, INTERVAL, 1000);

    if (!klines)
    {
        std::cerr << klines.errmsg << std::endl;
        return;
    }
    if (this->_EMACross.init(klines.v.klines) == EXIT_FAILURE)
    {
        std::cerr << this->_pair << ": Init error" << std::endl;
        return;
    }
    ws->klines(this->_pair.c_str(), INTERVAL, [this](const char *fl, int ec, std::string errmsg, binapi::ws::kline_t kline) {
        if (ec)
        {
            std::cerr << "subscribe klines error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
            return (false);
        }
        cross_e cross = this->_EMACross.update(kline);
        if (cross == cross_e::CROSS_UP)
        {
            std::cout << this->_pair << " Cross up -> Close time: " << Tools::Date::getDate(kline.T) << std::endl;
        }
        if (cross == cross_e::CROSS_DOWN)
        {
            std::cout << this->_pair << " Cross down -> Close time: " << Tools::Date::getDate(kline.T) << std::endl;
        }
        return (true);
    });
}

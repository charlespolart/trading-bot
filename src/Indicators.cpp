#include "Indicators.hpp"

Indicators::Indicators()
{
}

Indicators::Indicators(binapi::rest::api *api, binapi::ws::websockets_pool *ws, const std::string &pair, const std::string &interval) : _api(api),
                                                                                                                                        _ws(ws),
                                                                                                                                        _pair(pair),
                                                                                                                                        _interval(interval)
{
    this->_currentKline.t = 0;
}

Indicators::~Indicators()
{
}

/* PUBLIC */

binapi::ws::kline_t Indicators::getKline()
{
    return (this->_kline);
}

const ATR &Indicators::getATR()
{
    return (this->_ATR);
}

const EMACross &Indicators::getEMACross()
{
    return (this->_EMACross);
}

const EMA &Indicators::getEMA200()
{
    return (this->_EMA200);
}

const RSI &Indicators::getRSI()
{
    return (this->_RSI);
}

/*const SwingLow Indicators::getSwingLow()
{
    return (this->_swingLow);
}*/

int Indicators::init(Callback update_callback, size_t endTime)
{
    binapi::rest::api::result<binapi::rest::klines_t> klines = this->_api->klines(this->_pair, this->_interval, 1000, 0, endTime);

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

    this->_ATR.init(klines.v.klines, ATR_LENGTH);
    this->_EMACross.init(klines.v.klines, EMA_SHORT_BUY, EMA_LONG_BUY);
    this->_EMA200.init(klines.v.klines, 200);
    this->_RSI.init(klines.v.klines, RSI_LENGTH);
    // this->_swingLow.init(klines.v.klines, EMA_SWING_LOW);

    if (!HISTORY)
        this->candleSubscriber(update_callback);
    return (EXIT_SUCCESS);
}

void Indicators::update(const binapi::ws::kline_t &kline)
{
    this->_kline = kline;
    this->_ATR.update(kline);
    this->_EMACross.update(kline);
    this->_EMA200.update(kline);
    this->_RSI.update(kline);
    // this->_swingLow.update(kline);
}

/* PRIVATE */

void Indicators::candleSubscriber(Callback update_callback)
{
    std::cout << this->_pair << " - subscribe klines "
              << "(" << this->_interval << ")" << std::endl;
    Tools::Log::writeToFile("Debug", this->_pair + " - subscribe klines (" + this->_interval + ")");
    this->_candleHandler = this->_ws->klines(
        this->_pair.c_str(), this->_interval.c_str(), [this, update_callback](const char *fl, int ec, std::string errmsg, binapi::ws::kline_t kline)
        {
            if (ec)
            {
                std::cerr << this->_pair << " - subscribe klines "
                          << "(" << this->_interval << ")"
                          << " error: fl=" << fl << ", ec=" << ec << ", errmsg=" << errmsg << std::endl;
                Tools::Log::writeToFile("Debug", this->_pair + " - subscribe klines (" + this->_interval + ")"
                          + " error: fl=" + fl + ", ec=" + std::to_string(ec) + ", errmsg=" + errmsg);
                this->_ws->unsubscribe(this->_candleHandler);
                this->candleSubscriber(update_callback);
                return (false);
            }
            if (kline.t != this->_currentKline.t && this->_currentKline.t != 0)
            {
                update_callback(this->_currentKline);
            }
            this->_currentKline = kline;
            Tools::Log::writeToFile("candleSubsciber", this->_pair + " " + this->_interval + " received kline");
            return (true); });
}

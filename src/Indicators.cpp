#include "Indicators.hpp"

Indicators::Indicators()
{
}

Indicators::~Indicators()
{
}

/* PUBLIC */

statusEMACross_t Indicators::getEMACrossStatus()
{
    return (this->_EMACross.getStatus());
}

binapi::double_type Indicators::getRSIStatus()
{
    return (this->_RSI.getStatus());
}

void Indicators::init(const std::vector<binapi::rest::klines_t::kline_t> &klines)
{
    this->_EMACross.init(klines);
    this->_RSI.init(klines);
}

signal_e Indicators::fetchSignal()
{
    statusEMACross_t statusEMACross = this->_EMACross.getStatus();
    binapi::double_type RSI = this->_RSI.getStatus();

    if (statusEMACross.cross == cross_e::CROSS_UP && RSI >= RSI_MIN_BUY)
    {
        return (signal_e::BUY);
    }
    else if (statusEMACross.cross == cross_e::CROSS_DOWN)
    {
        return (signal_e::SELL);
    }
    return (signal_e::NONE);
}

void Indicators::update(const binapi::ws::kline_t &kline)
{
    this->_EMACross.update(kline);
    this->_RSI.update(kline);
}

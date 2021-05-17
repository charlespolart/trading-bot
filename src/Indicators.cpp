#include "Indicators.hpp"

Indicators::Indicators()
{
}

Indicators::~Indicators()
{
}

/* PUBLIC */

binapi::double_type Indicators::getATRStatus() const
{
    return (this->_ATR.getStatus());
}

statusEMACross_t Indicators::getEMACrossBuyStatus() const
{
    return (this->_EMACrossBuy.getStatus());
}

binapi::double_type Indicators::getRSIStatus() const
{
    return (this->_RSI.getStatus());
}

void Indicators::init(const std::vector<binapi::rest::klines_t::kline_t> &klines)
{
    this->_ATR.init(klines, ATR_LENGTH);
    this->_EMACrossBuy.init(klines, EMA_SHORT_BUY, EMA_LONG_BUY);
    this->_RSI.init(klines, RSI_LENGTH);
}

void Indicators::update(const binapi::ws::kline_t &kline)
{
    this->_ATR.update(kline);
    this->_EMACrossBuy.update(kline);
    this->_RSI.update(kline);
}

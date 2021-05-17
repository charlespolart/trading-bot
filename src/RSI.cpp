#include "RSI.hpp"

RSI::RSI() : _lastPrice(0.0)
{
}

RSI::~RSI()
{
}

/* PRIVATE */

void RSI::updateEMAs(binapi::double_type currentPrice, binapi::double_type previousPrice)
{
    binapi::double_type change = currentPrice - previousPrice;

    this->_EMAUp.update(change > 0.0 ? change : 0.0);
    this->_EMADown.update(change < 0.0 ? change * -1.0 : 0.0);
}

void RSI::update(binapi::double_type currentPrice)
{
    binapi::double_type RS = 0.0;

    this->updateEMAs(currentPrice, this->_lastPrice);
    if (this->_EMADown.getStatus() > 0.0)
        RS = this->_EMAUp.getStatus() / this->_EMADown.getStatus();
    this->_RSI = 100.0 - 100.0 / (1.0 + RS);
}

/* PUBLIC */

binapi::double_type RSI::getStatus() const
{
    return (this->_RSI);
}

void RSI::update(const binapi::ws::kline_t &kline)
{
    this->update(kline.c);
    this->_lastPrice = kline.c;
}

void RSI::init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int length)
{
    this->_EMAUp.init(length);
    this->_EMADown.init(length);
    for (size_t i = 0; i < klines.size(); ++i)
    {
        if (i > 0)
            this->updateEMAs(klines[i].close, klines[i - 1].close);
    }
    if (!klines.empty())
        this->_lastPrice = klines.back().close;
}

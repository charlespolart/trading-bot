#include "RSI.hpp"

RSI::RSI() : _RSI(0.0),
             _lastClose(0.0)
{
}

RSI::~RSI()
{
}

/* PRIVATE */

void RSI::update(binapi::double_type close)
{
    binapi::double_type change = close - _lastClose;
    binapi::double_type gain = change >= 0.0 ? change : 0.0;
    binapi::double_type loss = change < 0.0 ? -1.0 * change : 0.0;
    binapi::double_type RS = 0.0;

    this->_EMAGain.update(gain);
    this->_EMALoss.update(loss);
    if (this->_EMALoss.getStatus() > 0.0)
        RS = this->_EMAGain.getStatus() / this->_EMALoss.getStatus();
    this->_RSI = 100.0 - (100.0 / (1.0 + RS));
    _lastClose = close;
}

/* PUBLIC */

binapi::double_type RSI::getStatus() const
{
    return (this->_RSI);
}

void RSI::update(const binapi::ws::kline_t &kline)
{
    this->update(kline.c);
}

void RSI::init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int length)
{
    this->_EMAGain.setLength(length * 2 - 1);
    this->_EMALoss.setLength(length * 2 - 1);
    for (size_t i = 0; i < klines.size(); ++i)
        this->update(klines[i].close);
}

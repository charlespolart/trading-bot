#include "ATR.hpp"

ATR::ATR() : _length(0),
             _ATR(0.0),
             _prevClose(0.0)
{
}

ATR::~ATR()
{
}

/* PRIVATE */

binapi::double_type ATR::TR(binapi::double_type h, binapi::double_type l)
{
    binapi::double_type hL = h - l;
    binapi::double_type hPc = h - this->_prevClose;
    binapi::double_type lPc = l - this->_prevClose;

    return (this->_prevClose == 0.0 ? hL : static_cast<binapi::double_type>(std::max(hL, std::max(hPc, lPc))));
}

void ATR::update(binapi::double_type h, binapi::double_type l, binapi::double_type c)
{
    binapi::double_type TR = this->TR(h, l);

    this->_ATR = ((this->_ATR * (this->_length - 1.0)) + TR) / this->_length;
    this->_prevClose = c;
}

/* PUBLIC */

binapi::double_type ATR::getStatus() const
{
    return (this->_ATR);
}

void ATR::init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int length)
{
    this->_length = length;
    for (size_t i = 0; i < klines.size(); ++i)
    {
        if (i == 0)
        {
            this->_ATR = TR(klines[i].high, klines[i].low);
            this->_prevClose = klines[i].close;
        }
        else
        {
            this->update(klines[i].high, klines[i].low, klines[i].close);
        }
    }
}

void ATR::update(const binapi::ws::kline_t &kline)
{
    this->update(kline.h, kline.l, kline.c);
}

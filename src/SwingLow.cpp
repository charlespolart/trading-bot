#include "SwingLow.hpp"

SwingLow::SwingLow() : _lastLowest(0.0)
{
}

SwingLow::~SwingLow()
{
}

/* PUBLIC */

binapi::double_type SwingLow::getStatus() const
{
    return (this->_lastLowest);
}

void SwingLow::init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int EMALength)
{
    this->_EMA.init(EMALength);
    for (size_t i = 0; i < klines.size(); ++i)
        this->_EMA.update(klines[i].close);
    for (std::vector<binapi::rest::klines_t::kline_t>::const_reverse_iterator it = klines.rbegin(); it != klines.rend() && it->close <= this->_EMA.getStatus(); ++it)
        this->_lowList.push_front(it->close);
}

void SwingLow::update(const binapi::ws::kline_t &kline)
{
    this->_EMA.update(kline.c);
    if (kline.l <= this->_EMA.getStatus())
        this->_lowList.push_back(kline.l);
    else if (!this->_lowList.empty())
    {
        this->_lowList.sort();
        this->_lastLowest = this->_lowList.front();
        this->_lowList.clear();
    }
}

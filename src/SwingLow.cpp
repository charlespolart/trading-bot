#include "SwingLow.hpp"

SwingLow::SwingLow() : _lastLowest(0.0)
{
}

SwingLow::~SwingLow()
{
}

/* PRIVATE */

void SwingLow::update(binapi::double_type low)
{
    if (low <= this->_EMA.getStatus())
        this->_lowList.push_back(low);
    else if (!this->_lowList.empty())
    {
        this->_lowList.sort();
        this->_lastLowest = this->_lowList.front();
        this->_lowList.clear();
    }
}

/* PUBLIC */

binapi::double_type SwingLow::getStatus() const
{
    return (this->_lastLowest);
}

void SwingLow::init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int EMALength)
{
    this->_EMA.init(klines, EMALength);
    for (size_t i = 0; i < klines.size(); ++i)
        this->update(klines[i].low);
}

void SwingLow::update(const binapi::ws::kline_t &kline)
{
    this->_EMA.update(kline.c);
    this->update(kline.l);
}

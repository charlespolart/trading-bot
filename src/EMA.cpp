#include "EMA.hpp"

EMA::EMA() : _length(0),
             _EMA(0.0)
{
}

EMA::~EMA()
{
}

/* PUBLIC */

binapi::double_type EMA::getStatus() const
{
    return (this->_EMA);
}

bool EMA::isTrending() const
{
    std::list<binapi::double_type>::const_reverse_iterator last = this->_buffer.rbegin();
    std::list<binapi::double_type>::const_reverse_iterator prev = this->_buffer.rbegin();

    prev++;
    return (*last > *prev);
}

void EMA::setLength(int length)
{
    this->_length = length;
}

void EMA::init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int length)
{
    this->_length = length;
    for (size_t i = 0; i < klines.size(); ++i)
        this->update(klines[i].close);
}

void EMA::update(binapi::double_type value)
{
    if (this->_EMA == 0.0)
        this->_EMA = value;
    else
        this->_EMA += (value - this->_EMA) * (2.0 / (this->_length + 1.0));
    this->_buffer.push_back(this->_EMA);
    if (this->_buffer.size() > EMA_BUFFER_SIZE)
        this->_buffer.pop_front();
}

void EMA::update(const binapi::ws::kline_t &kline)
{
    this->update(kline.c);
}

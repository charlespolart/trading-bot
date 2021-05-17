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

void EMA::init(int length)
{
    this->_length = length;
}

void EMA::update(binapi::double_type value)
{
    if (this->_EMA == 0.0)
        this->_EMA = value;
    else
        this->_EMA += (value - this->_EMA) * (2.0 / (this->_length + 1.0));
}

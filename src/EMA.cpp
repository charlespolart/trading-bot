#include "EMA.hpp"

EMA::EMA() : _EMA(0.0)
{
}

EMA::~EMA()
{
}

binapi::double_type EMA::getStatus()
{
    return (this->_EMA);
}

void EMA::update(binapi::double_type value, int period)
{
    if (this->_EMA == 0.0)
    {
        this->_EMA = value;
        return;
    }
    this->_EMA += (value - this->_EMA) * (2.0 / (period + 1.0));
}

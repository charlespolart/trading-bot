#include "EMACross.hpp"

EMACross::EMACross() : _cross(cross_e::NONE)
{
}

/* PRIVATE */

void EMACross::updateCross(binapi::double_type prevShortaverage, binapi::double_type prevLongAverage)
{
    if (prevShortaverage == prevLongAverage)
        return;
    if ((prevShortaverage < prevLongAverage) && (this->_EMA.currentShort > this->_EMA.currentLong))
        this->_cross = cross_e::CROSS_UP;
    else if ((prevShortaverage > prevLongAverage) && (this->_EMA.currentShort < this->_EMA.currentLong))
        this->_cross = cross_e::CROSS_DOWN;
}

void EMACross::addNewCandle()
{
    binapi::double_type prevShortaverage = this->_EMA.currentShort;
    binapi::double_type prevLongAverage = this->_EMA.currentLong;

    this->_EMA.update(this->_currentCandle.closePrice);
    this->updateCross(prevShortaverage, prevLongAverage);
}

/* PUBLIC */

cross_e EMACross::update(const binapi::ws::kline_t &kline)
{
    this->_cross = cross_e::NONE;
    if (kline.t != this->_currentCandle.startTime)
        this->addNewCandle();
    this->_currentCandle = candle_t{kline.t, kline.c};
    return (this->_cross);
}

int EMACross::init(const std::vector<binapi::rest::klines_t::kline_t> &klines)
{
    if (klines.size() < 2)
        return (EXIT_FAILURE);
    for (size_t i = 0; i < klines.size() - 1; ++i)
        this->_EMA.update(klines[i].close);
    this->_currentCandle = candle_t{klines.back().start_time, klines.back().close};
    return (EXIT_SUCCESS);
}

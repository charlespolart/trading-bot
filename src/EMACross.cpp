#include "EMACross.hpp"

EMACross::EMACross() : _cross(cross_e::NONE)
{
}

EMACross::~EMACross()
{
}

/* PRIVATE */

void EMACross::updateCross(binapi::double_type prevShortaverage, binapi::double_type prevLongAverage)
{
    if (prevShortaverage == prevLongAverage)
        return;
    if ((prevShortaverage < prevLongAverage) && (this->_EMAShort.getStatus() > this->_EMALong.getStatus()))
        this->_cross = cross_e::CROSS_UP;
    else if ((prevShortaverage > prevLongAverage) && (this->_EMAShort.getStatus() < this->_EMALong.getStatus()))
        this->_cross = cross_e::CROSS_DOWN;
}

void EMACross::addNewCandle(const binapi::ws::kline_t &kline)
{
    binapi::double_type prevShortaverage = this->_EMAShort.getStatus();
    binapi::double_type prevLongAverage = this->_EMALong.getStatus();

    this->_EMAShort.update(kline.c);
    this->_EMALong.update(kline.c);
    this->updateCross(prevShortaverage, prevLongAverage);
}

/* PUBLIC */

statusEMACross_t EMACross::getStatus() const
{
    statusEMACross_t status;

    status.cross = this->_cross;
    status.EMAShort = this->_EMAShort;
    status.EMALong = this->_EMALong;
    return (status);
}

bool EMACross::crossed() const
{
    return (this->_EMAShort.getStatus() > this->_EMALong.getStatus());
}

void EMACross::update(const binapi::ws::kline_t &kline)
{
    this->_cross = cross_e::NONE;
    this->addNewCandle(kline);
}

void EMACross::init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int EMAShort, int EMALong)
{
    this->_EMAShort.init(klines, EMAShort);
    this->_EMALong.init(klines, EMALong);
}

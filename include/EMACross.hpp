#ifndef EMACROSS_HPP
#define EMACROSS_HPP

#include <binapi/api.hpp>

#include "Params.h"
#include "EMA.hpp"

enum class cross_e
{
    NONE,
    CROSS_UP,
    CROSS_DOWN
};

struct statusEMACross_t
{
    cross_e cross = cross_e::NONE;
    EMA EMAShort;
    EMA EMALong;
};

class EMACross
{
public:
    EMACross();
    ~EMACross();

public:
    statusEMACross_t getStatus() const;
    bool crossed() const;
    void update(const binapi::ws::kline_t &kline);
    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int EMAShort, int EMALong);

private:
    void updateCross(binapi::double_type prevShortaverage, binapi::double_type prevLongAverage);
    void computeEMA();
    void addNewCandle(const binapi::ws::kline_t &kline);

private:
    EMA _EMAShort;
    EMA _EMALong;
    cross_e _cross;
};

#endif // EMACROSS_HPP

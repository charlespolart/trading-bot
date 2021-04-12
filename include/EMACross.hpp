#ifndef EMACROSS_HPP
#define EMACROSS_HPP

#include <binapi/api.hpp>

#include "Params.h"
#include "Tools.hpp"

enum cross_e
{
    NONE,
    CROSS_UP,
    CROSS_DOWN
};

struct candle_t
{
    size_t startTime = 0;
    binapi::double_type openPrice = 0.0;
    binapi::double_type closePrice = 0.0;
};

struct EMA_t
{
    void update(binapi::double_type close)
    {
        if (this->currentShort == 0.0 && this->currentLong == 0.0)
        {
            this->currentShort = close;
            this->currentLong = close;
            return;
        }
        this->currentShort += (close - this->currentShort) * (2.0 / (SHORT + 1.0));
        this->currentLong += (close - this->currentLong) * (2.0 / (LONG + 1.0));
    }
    binapi::double_type currentShort = 0.0;
    binapi::double_type currentLong = 0.0;
};

struct result_t
{
    cross_e cross;
    EMA_t EMA;
    candle_t candle;
};

class EMACross
{
public:
    EMACross();

public:
    result_t update(const binapi::ws::kline_t &kline);
    int init(const std::vector<binapi::rest::klines_t::kline_t> &klines);

private:
    void updateCross(binapi::double_type prevShortaverage, binapi::double_type prevLongAverage);
    void computeEMA();
    void addNewCandle();

private:
    EMA_t _EMA;
    cross_e _cross;

    candle_t _currentCandle;
};

#endif // EMACROSS_HPP

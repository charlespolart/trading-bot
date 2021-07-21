#ifndef RSI_HPP
#define RSI_HPP

#include <list>

#include <binapi/api.hpp>

#include "Params.h"
#include "EMA.hpp"

class RSI
{
public:
    RSI();
    ~RSI();

public:
    binapi::double_type getStatus() const;
    void update(const binapi::ws::kline_t &kline);
    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int length);

private:
    void updateEMAs(binapi::double_type currentPrice, binapi::double_type previousPrice);
    void update(binapi::double_type close);

private:
    binapi::double_type _RSI;
    EMA _EMAGain;
    EMA _EMALoss;
    binapi::double_type _lastClose;
};

#endif // RSI_HPP

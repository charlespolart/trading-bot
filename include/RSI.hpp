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
    binapi::double_type getStatus();
    void update(const binapi::ws::kline_t &kline);
    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines);

private:
    void updateEMAs(binapi::double_type currentPrice, binapi::double_type previousPrice);
    void update(binapi::double_type currentPrice);

private:
    binapi::double_type _RSI;
    EMA _EMAUp;
    EMA _EMADown;
    binapi::double_type _lastPrice;
};

#endif // RSI_HPP

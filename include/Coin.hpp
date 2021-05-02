#ifndef COIN_HPP
#define COIN_HPP

#include <binapi/websocket.hpp>

#include "Params.h"
#include "Database.hpp"
#include "Indicators.hpp"
#include "Tools.hpp"

class Coin
{
public:
    Coin() = delete;
    Coin(const std::string &pair, binapi::double_type stepSize);
    ~Coin();

public:
    binapi::rest::new_order_info_full_t getBuyOrderInfo() const;

public:
    void updateCallback(const binapi::ws::kline_t &kline);
    int init(binapi::rest::api *api, size_t endTime = 0);

public:
    std::string _pair;
    binapi::double_type _stepSize;

private:
    Indicators _indicators;
    binapi::rest::new_order_info_full_t _buyOrderInfo;
    binapi::rest::new_order_info_full_t _sellOrderInfo;
};

#endif // COIN_HPP

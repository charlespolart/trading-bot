#ifndef COIN_HPP
#define COIN_HPP

#include <binapi/websocket.hpp>

#include "Params.h"
#include "Database.hpp"
#include "EMACross.hpp"
#include "Tools.hpp"

class Coin
{
public:
    Coin() = delete;
    Coin(const std::string &pair, binapi::double_type stepSize);
    ~Coin();

public:
    void updateCallback(const binapi::ws::kline_t &kline);
    int init(binapi::rest::api *api, size_t endTime = 0);

public:
    std::string _pair;
    binapi::double_type _stepSize;

private:
    EMACross _EMACross;
};

#endif // COIN_HPP

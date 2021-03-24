#ifndef COIN_HPP
#define COIN_HPP

#include <binapi/websocket.hpp>

#include "Params.h"
#include "EMACross.hpp"
#include "Tools.hpp"

class Coin
{
public:
    Coin() = delete;
    Coin(const std::string &pair, binapi::double_type stepSize);
    ~Coin();

public:
    void init(binapi::rest::api *api, binapi::ws::websockets_pool *ws);

public:
    std::string _pair;
    binapi::double_type _stepSize;

private:
    EMACross _EMACross;
};

#endif // COIN_HPP

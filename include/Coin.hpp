#ifndef COIN_HPP
#define COIN_HPP

#include <boost/asio/post.hpp>

#include <binapi/websocket.hpp>

#include "Params.h"
#include "Database.hpp"
#include "Indicators.hpp"
#include "Tools.hpp"
#include "User.hpp"


enum class signal_e
{
    NONE,
    BUY,
    SELL
};

class Coin
{
public:
    Coin() = delete;
    Coin(boost::asio::io_context &ioctx, const std::string &pair, binapi::double_type stepSize, const std::vector<User *> &users);
    ~Coin();

public:
    binapi::rest::new_order_info_full_t getBuyOrderInfo() const;

public:
    void updateCallback(const binapi::ws::kline_t &kline);
    int init(binapi::rest::api *api, size_t endTime = 0);

private:
    signal_e fetchSignal(const binapi::ws::kline_t &kline);
    void buy(const binapi::ws::kline_t &kline);
    void sell(const binapi::ws::kline_t &kline);

public:
    std::string _pair;

private:
    boost::asio::io_context &_ioctx;
    binapi::double_type _stepSize;
    Indicators _indicators;
    const std::vector<User *> &_users;
};

#endif // COIN_HPP

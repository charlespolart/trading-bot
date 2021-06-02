#ifndef COIN_HPP
#define COIN_HPP

#include <boost/asio/post.hpp>

#include <binapi/websocket.hpp>

#include "Params.h"
#include "Database.hpp"
#include "Indicators.hpp"
#include "Tools.hpp"
#include "User.hpp"

enum signal_e
{
    NONE,
    BUY,
    SELL
};

struct status_t
{
    bool bought = false;
    binapi::double_type currentStopLoss = 0.0;
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
    signal_e fetchSignal();
    void buy(const binapi::ws::kline_t &kline);
    void sell(const binapi::ws::kline_t &kline);
    void setStopLoss(binapi::double_type value);
    void checkStopLossStatus(const binapi::ws::kline_t &kline);
    void writeTransaction(const std::string &type, const binapi::ws::kline_t &kline);

public:
    std::string _pair;

private:
    boost::asio::io_context &_ioctx;
    binapi::double_type _stepSize;
    Indicators _indicators;
    const std::vector<User *> &_users;
    status_t _status;
};

#endif // COIN_HPP

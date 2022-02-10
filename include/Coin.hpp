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
    Coin(boost::asio::io_context &ioctx, binapi::rest::api *api, binapi::ws::websockets_pool *ws, const std::string &pair, binapi::double_type stepSize, const std::vector<User *> &users);
    ~Coin();

public:
    binapi::rest::new_order_info_full_t getBuyOrderInfo() const;

public:
    void update_callback(const binapi::ws::kline_t &kline);
    int init(size_t endTime = 0);

private:
    bool validateSignal();
    signal_e fetchSignal();
    void trade(const binapi::ws::kline_t &kline);
    void buy(const binapi::ws::kline_t &kline);
    void sell(const binapi::ws::kline_t &kline);
    void setStopLoss(binapi::double_type value);
    //void updateStopLoss();
    void writeTransaction(const std::string &type, const binapi::ws::kline_t &kline);

public:
    std::string _pair;

private:
    boost::asio::io_context &_ioctx;
    binapi::rest::api *_api;
    binapi::ws::websockets_pool *_ws;
    binapi::double_type _stepSize;
    std::map<std::string, Indicators> _indicators;
    const std::vector<User *> &_users;
    status_t _status;
};

#endif // COIN_HPP

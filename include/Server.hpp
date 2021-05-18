#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <thread>
#include <boost/asio/io_context.hpp>

#include <binapi/api.hpp>
#include <binapi/websocket.hpp>

#include "Params.h"
#include "Coin.hpp"
#include "Database.hpp"
#include "FetchUsers.hpp"
#include "User.hpp"

class Server
{
public:
    Server();
    ~Server();

public:
    int run();

private:
    int fetchCoins();
    void contextRun_thread();
    void candle_callback(Coin *coin);
    int runHistory();
    int runProduction();

private:
    binapi::rest::api *_api;
    binapi::ws::websockets_pool *_ws;
    boost::asio::io_context _ioctx;
    std::vector<User *> _users;
    std::vector<Coin *> _coins;
    std::map<std::string, binapi::ws::kline_t> _currentKlines;
};

#endif // SERVER_HPP

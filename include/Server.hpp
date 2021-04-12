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

class Server
{
public:
    Server();
    ~Server();

public:
    int runEMAHistory();
    int runEMA();
    int run();

private:
    int fetchCoins();
    void contextRun_thread();

private:
    binapi::rest::api *_api;
    binapi::ws::websockets_pool *_ws;
    boost::asio::io_context _ioctx;
    std::vector<Coin *> _coins;
};

#endif // SERVER_HPP

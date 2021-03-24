#include "Server.hpp"

Server::Server()
{
    this->_api = new binapi::rest::api(
        this->_ioctx, "api.binance.com", "443", "", "", 10000);
    this->_ws = new binapi::ws::websockets_pool(
        this->_ioctx, "stream.binance.com", "9443");
}

Server::~Server()
{
    if (this->_api)
        delete this->_api;
    if (this->_ws)
        delete this->_ws;
    for (size_t i = 0; i < this->_coins.size(); ++i)
    {
        if (this->_coins[i])
            delete this->_coins[i];
    }
}

/* PRIVATE */

int Server::fetchCoins()
{
    std::cout << "Fetching coins..." << std::endl;
    auto exchange_info = this->_api->exchange_info();

    if (!exchange_info)
    {
        std::cerr << exchange_info.errmsg << std::endl;
        return (EXIT_FAILURE);
    }
    for (auto it = exchange_info.v.symbols.begin(); it != exchange_info.v.symbols.end(); ++it)
    {
        if (it->second.quoteAsset == "BTC")
        {
            std::string pair = it->first;
            binapi::double_type stepSize = it->second.get_filter_lot().stepSize;
            //if (it->first == "ETHBTC")
            this->_coins.emplace_back(new Coin(pair, stepSize));
        }
    }
    return (EXIT_SUCCESS);
}

void Server::contextRun_thread()
{
    while (1)
    {
        this->_ioctx.run();
        this->_ioctx.restart();
    }
}

/* PUBLIC */

int Server::run()
{
    std::thread contextThread = std::thread(&Server::contextRun_thread, this);

    if (SHORT >= LONG || std::min(SHORT, LONG) <= 0)
    {
        std::cerr << "SHORT must be smaller than LONG (Range [1-1000])" << std::endl;
        return (EXIT_FAILURE);
    }
    if (this->fetchCoins() == EXIT_FAILURE)
        return (EXIT_FAILURE);
    for (size_t i = 0; i < this->_coins.size(); ++i)
    {
        this->_coins[i]->init(this->_api, this->_ws);
    }
    if (contextThread.joinable())
        contextThread.join();
    return (EXIT_SUCCESS);
}

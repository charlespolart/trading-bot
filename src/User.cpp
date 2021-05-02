#include "User.hpp"

User::User(boost::asio::io_context &ioctx, const std::string &name,
           const std::string &apiKey, const std::string &apiSecret,
           const std::string &btcAmount) : _ioctx(ioctx),
                                           _name(name),
                                           _btcAmount(btcAmount)

{
    this->_api = new binapi::rest::api(
        this->_ioctx, "api.binance.com", "443",
        apiKey, apiSecret,
        10000);
}

User::~User()
{
    if (this->_api)
        delete this->_api;
}

/* PUBLIC */

std::string User::getName() const
{
    return (this->_name);
}

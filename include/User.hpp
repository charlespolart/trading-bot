#ifndef USER_HPP
#define USER_HPP

#include <boost/asio/io_context.hpp>

#include <binapi/api.hpp>

class User
{
public:
    User() = delete;
    User(boost::asio::io_context &ioctx, const std::string &name,
         const std::string &apiKey, const std::string &apiSecret,
         const std::string &btcAmount);
    ~User();

public:
    std::string getName() const;

private:
    binapi::rest::api *_api;
    boost::asio::io_context &_ioctx;

    std::string _name;
    std::string _btcAmount;
};

#endif // USER_HPP

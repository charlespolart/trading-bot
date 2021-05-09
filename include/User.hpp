#ifndef USER_HPP
#define USER_HPP

#include <thread>

#include <boost/asio/io_context.hpp>

#include <binapi/api.hpp>

#include "Tools.hpp"

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
    binapi::rest::new_order_info_full_t buy(const std::string &coin);
    void sell(const std::string &coin, binapi::double_type stepSize);

private:
    binapi::rest::api *_api;
    boost::asio::io_context &_ioctx;

    std::string _name;
    std::string _btcAmount;
    std::map<std::string, binapi::rest::new_order_info_full_t> _boughtList;
    std::mutex _mutex;
};

#endif // USER_HPP

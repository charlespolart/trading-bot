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

binapi::rest::new_order_info_full_t User::buy(const std::string &pair)
{
    binapi::rest::new_order_info_full_t bought;

    binapi::rest::api::result<binapi::rest::new_order_resp_type> res = this->_api->new_order(
        pair,
        binapi::e_side::buy,
        binapi::e_type::market,
        binapi::e_time::GTC,
        "",
        this->_btcAmount,
        "",
        "",
        "",
        "");
    if (!res)
        std::cout << "Buy: " << res.errmsg << std::endl;
    else
    {
        bought = res.v.get_response_full();
        this->_mutex.lock();
        this->_boughtList[pair] = bought;
        this->_mutex.unlock();
        std::cout << "Buy: " << bought << std::endl;
    }
    return (bought);
}

void User::sell(const std::string &pair, binapi::double_type stepSize)
{
    binapi::double_type amount = 0.0;

    if (this->_boughtList.find(pair) == this->_boughtList.end())
        return;
    this->_mutex.lock();
    binapi::rest::new_order_info_full_t bought = this->_boughtList[pair];
    this->_boughtList.erase(pair);
    this->_mutex.unlock();

    for (size_t i = 0; i < bought.fills.size(); ++i)
    {
        if (bought.fills[i].commissionAsset == "BNB")
            amount += bought.fills[i].qty;
        else
            amount += bought.fills[i].qty - bought.fills[i].commission;
    }
    binapi::rest::api::result<binapi::rest::new_order_resp_type> res = this->_api->new_order(
        pair,
        binapi::e_side::sell,
        binapi::e_type::market,
        binapi::e_time::GTC,
        Tools::Convert::to_string(amount, Tools::Convert::getPrecision(stepSize)),
        "",
        "",
        "",
        "",
        "");
    if (!res)
        std::cout << "Sell: " << res.errmsg << std::endl;
    else
    {
        binapi::rest::new_order_info_full_t sold = res.v.get_response_full();
        std::cout << "Sell: " << sold << std::endl;
    }
}

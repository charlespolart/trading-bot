#ifndef INDICATORS_HPP
#define INDICATORS_HPP

#include <binapi/websocket.hpp>

#include "ATR.hpp"
#include "EMACross.hpp"
#include "RSI.hpp"
#include "SwingLow.hpp"

typedef std::function<void(const binapi::ws::kline_t &)> Callback;

class Indicators
{
public:
    Indicators();
    Indicators(binapi::rest::api *api, binapi::ws::websockets_pool *ws, const std::string &pair, const std::string &interval);
    ~Indicators();

public:
    binapi::ws::kline_t getKline();
    const ATR &getATR();
    const EMACross &getEMACross();
    const EMA &getEMA200();
    const RSI &getRSI();
    //const SwingLow &getSwingLow();

    int init(Callback update_callback, size_t endTime);
    void update(const binapi::ws::kline_t &kline);

private:
    void candleSubscriber(Callback update_callback);

private:
    binapi::rest::api *_api;
    binapi::ws::websockets_pool *_ws;
    std::string _pair;
    std::string _interval;
    binapi::ws::websockets_pool::handle _candleHandler;
    binapi::ws::kline_t _currentKline;

    binapi::ws::kline_t _kline;
    ATR _ATR;
    EMACross _EMACross;
    EMA _EMA200;
    RSI _RSI;
    //SwingLow _swingLow;
};

#endif // INDICATORS_HPP

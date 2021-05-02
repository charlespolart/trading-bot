#ifndef INDICATORS_HPP
#define INDICATORS_HPP

#include "EMACross.hpp"
#include "RSI.hpp"

enum class signal_e
{
    NONE,
    BUY,
    SELL
};

class Indicators
{
public:
    Indicators();
    ~Indicators();

public:
    statusEMACross_t getEMACrossStatus();
    binapi::double_type getRSIStatus();
    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines);
    signal_e fetchSignal();
    void update(const binapi::ws::kline_t &kline);

private:
    EMACross _EMACross;
    RSI _RSI;
};

#endif // INDICATORS_HPP

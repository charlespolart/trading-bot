#ifndef INDICATORS_HPP
#define INDICATORS_HPP

#include "ATR.hpp"
#include "EMACross.hpp"
#include "RSI.hpp"
#include "SwingLow.hpp"

class Indicators
{
public:
    Indicators();
    ~Indicators();

public:
    binapi::double_type getATRStatus() const;
    statusEMACross_t getEMACrossBuyStatus() const;
    binapi::double_type getRSIStatus() const;
    binapi::double_type getSwingLowStatus() const;
    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines);
    void update(const binapi::ws::kline_t &kline);

private:
    ATR _ATR;
    EMACross _EMACrossBuy;
    RSI _RSI;
    SwingLow _swingLow;
};

#endif // INDICATORS_HPP

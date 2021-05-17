#ifndef INDICATORS_HPP
#define INDICATORS_HPP

#include "EMACross.hpp"
#include "Resistance.hpp"
#include "RSI.hpp"

class Indicators
{
public:
    Indicators();
    ~Indicators();

public:
    statusEMACross_t getEMACrossBuyStatus() const;
    statusEMACross_t getEMACrossSellStatus() const;
    binapi::double_type getResistanceStatus() const;
    binapi::double_type getRSIStatus() const;
    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines);
    void update(const binapi::ws::kline_t &kline);

private:
    EMACross _EMACrossBuy;
    EMACross _EMACrossSell;
    Resistance _resistance;
    RSI _RSI;
};

#endif // INDICATORS_HPP

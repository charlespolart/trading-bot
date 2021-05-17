#ifndef RESISTANCE_HPP
#define RESISTANCE_HPP

#include <binapi/api.hpp>

class Resistance
{
public:
    Resistance();
    ~Resistance();

public:
    binapi::double_type getStatus() const;
    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int length);
    void update(const binapi::ws::kline_t &kline);

private:
    int _length;
    std::list<binapi::double_type> _highPrices;
};

#endif // RESISTANCE_HPP

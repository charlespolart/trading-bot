#ifndef SWINGLOW_HPP
#define SWINGLOW_HPP

#include "EMA.hpp"

class SwingLow
{
public:
    SwingLow();
    ~SwingLow();

public:
    binapi::double_type getStatus() const;
    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int EMALength);
    void update(const binapi::ws::kline_t &kline);

private:
    void update(binapi::double_type low);

private:
    EMA _EMA;
    std::list<binapi::double_type> _lowList;
    binapi::double_type _lastLowest;
};

#endif // SWINGLOW_HPP

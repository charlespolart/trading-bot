#ifndef ATR_HPP
#define ATR_HPP

#include <binapi/api.hpp>

#include "Params.h"

class ATR
{
public:
    ATR();
    ~ATR();

public:
    binapi::double_type getStatus() const;
    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int length);
    void update(const binapi::ws::kline_t &kline);

private:
    binapi::double_type TR(binapi::double_type h, binapi::double_type l);
    void update(binapi::double_type h, binapi::double_type l, binapi::double_type c);

private:
    int _length;
    binapi::double_type _ATR;
    binapi::double_type _prevClose;
};

#endif // ATR_HPP

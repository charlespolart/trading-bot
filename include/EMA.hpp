#ifndef EMA_HPP
#define EMA_HPP

#include <binapi/api.hpp>

#include "Params.h"

#define EMA_BUFFER_SIZE 20

class EMA
{
public:
    EMA();
    ~EMA();

public:
    void setLength(int length);

public:
    binapi::double_type getStatus() const;
    bool isTrending() const;

    void init(const std::vector<binapi::rest::klines_t::kline_t> &klines, int length);
    void update(binapi::double_type value);
    void update(const binapi::ws::kline_t &kline);

private:
    int _length;
    binapi::double_type _EMA;
    std::list<binapi::double_type> _buffer;
};

#endif // EMA_HPP

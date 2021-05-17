#ifndef EMA_HPP
#define EMA_HPP

#include <binapi/api.hpp>

#include "Params.h"

class EMA
{
public:
    EMA();
    ~EMA();

public:
    binapi::double_type getStatus() const;
    void init(int length);
    void update(binapi::double_type value);

private:
    int _length;
    binapi::double_type _EMA;
};

#endif // EMA_HPP

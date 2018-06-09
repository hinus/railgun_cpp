#ifndef _HI_INTEGER_HPP
#define _HI_INTEGER_HPP

#include "hiObject.hpp"

class HiInteger : public HiObject {
private:
    int _value;

public:
    HiInteger(int x);

    int value() { return _value; }

    HiInteger* operator + (HiInteger& right) {
        return new HiInteger(_value + right._value);
    }

    HiInteger* operator - (HiInteger& right) {
        return new HiInteger(_value - right._value);
    }

    HiInteger* operator * (HiInteger& right) {
        return new HiInteger(_value * right._value);
    }

    HiInteger* operator / (HiInteger& right) {
        return new HiInteger(_value / right._value);
    }

    bool operator == (HiInteger& right) {
        return _value == right._value;
    }

    bool operator < (HiInteger& right) {
        return _value < right._value;
    }
};

#endif

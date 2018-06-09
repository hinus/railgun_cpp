#ifndef _HI_STRING_HPP
#define _HI_STRING_HPP

#include "hiObject.hpp"

class HiString : public HiObject {
private:
    const char* _value;

public:
    HiString(const char * x) : 
        _value(x) { }

    virtual void print() {
        std::cout << _value;
    }
};

#endif

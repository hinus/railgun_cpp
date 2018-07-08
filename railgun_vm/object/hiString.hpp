#ifndef _HI_STRING_HPP
#define _HI_STRING_HPP

#include "hiObject.hpp"

class HiString : public HiObject {
private:
    const char* _value;
	int         _length;

public:
    HiString(const char * x) : 
        _value(x),
		_length(0)
		{ }

	HiString(const char * x, const int length) : 
		_value(x),
		_length(length) {}

    virtual void print() {
        std::cout << _value;
    }

	const char* value() {return _value;}
	int length()  {return _length;}
};

#endif

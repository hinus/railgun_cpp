#ifndef _HI_STRING_HPP
#define _HI_STRING_HPP

#include "hiObject.hpp"

class StringKlass : public Klass {
private:
	StringKlass() {}
	static StringKlass* instance;

public:
	static StringKlass* get_instance();

	virtual void print(HiObject* obj);
};

class HiString : public HiObject {
private:
    const char* _value;
	int         _length;

public:
    HiString(const char * x) : 
        _value(x),
		_length(0) {
			set_klass(StringKlass::get_instance());
		}

	HiString(const char * x, const int length) : 
		_value(x),
		_length(length) {
			set_klass(StringKlass::get_instance());
		}

	const char* value() {return _value;}
	int length()  {return _length;}
};

#endif

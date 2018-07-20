#ifndef _HI_STRING_HPP
#define _HI_STRING_HPP

#include "hiObject.hpp"
#include <string.h>

class StringKlass : public Klass {
private:
    StringKlass() {}
    static StringKlass* instance;

public:
    static StringKlass* get_instance();

    virtual HiObject* equal    (HiObject* x, HiObject* y);

    virtual void print(HiObject* obj);

    virtual void oops_do(OopClosure* closure, HiObject* obj);
    virtual size_t size();
};

class HiString : public HiObject {
private:
    char* _value;
    int   _length;

public:
    HiString(const char * x);
    HiString(const char * x, const int length);

    const char* value()     { return _value; }
    char** value_address()  { return &_value; };
    int length()            { return _length; }

    void* operator new(size_t size);
};

#endif

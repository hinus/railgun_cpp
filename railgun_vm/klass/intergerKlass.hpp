#ifndef _INTEGER_KLASS_HPP
#define _INTEGER_KLASS_HPP

#include "klass/klass.hpp"

class HiObject;
class HiInteger;

class IntegerKlass : public Klass {
public:
    virtual void print(HiObject* obj);

    virtual HiObject* add(HiObject* x, HiObject* y);
    virtual HiObject* sub(HiObject* x, HiObject* y);
    virtual HiObject* mul(HiObject* x, HiObject* y);
    virtual HiObject* div(HiObject* x, HiObject* y);

    // alloacte instances
    virtual HiObject* allocate_instance();
};

#endif

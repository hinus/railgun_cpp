#ifndef _HI_OBJECT_HPP
#define _HI_OBJECT_HPP

#include "klass/klass.hpp"
#include <assert.h>

class HiString;

class HiObject {
private:
    long _mark_word;
    Klass* _klass;

public:
    HiObject() {}

    Klass* klass()             { assert(_klass != NULL); return _klass; }
    void set_klass(Klass* x)   { _klass = x; }

	void print();
    HiObject* greater  (HiObject* x);
    HiObject* less     (HiObject* x);
    HiObject* equal    (HiObject* x);
    HiObject* not_equal(HiObject* x);
    HiObject* ge       (HiObject* x);
    HiObject* le       (HiObject* x);

    HiObject* add(HiObject* x);
    HiObject* sub(HiObject* x);
    HiObject* mul(HiObject* x);
    HiObject* div(HiObject* x);
    HiObject* mod(HiObject* x);

    HiObject* getattr(HiObject* x);
};

#endif

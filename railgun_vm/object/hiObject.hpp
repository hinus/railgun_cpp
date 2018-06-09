#ifndef _HI_OBJECT_HPP
#define _HI_OBJECT_HPP

#include "klass/klass.hpp"

class HiObject {
private:
    uintptr_t _mark_word;
    Klass* _klass;

public:
    HiObject() {}

    Klass* klass()             { return _klass; }
    void set_klass(Klass* x)   { _klass = x; }
};

#endif

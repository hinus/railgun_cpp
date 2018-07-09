#ifndef _UNIVERSE_HPP
#define _UNIVERSE_HPP

#include "klass/klass.hpp"

class HiInteger;

class Universe {
public:
    static Klass* _object_klass;
    static Klass* _type_klass;
    static Klass* _int_klass;

    static HiInteger* HiTrue;
    static HiInteger* HiFalse;

public:
    static void genesis();
};

#endif
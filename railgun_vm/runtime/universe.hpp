#ifndef _UNIVERSE_HPP
#define _UNIVERSE_HPP

#include "klass/klass.hpp"

class HiInteger;
class HiNoneObject;

class Universe {
public:
    static Klass* _object_klass;
    static Klass* _type_klass;
    static Klass* _int_klass;

    static HiInteger* HiTrue;
    static HiInteger* HiFalse;

	static HiNoneObject* HiNone;

public:
    static void genesis();
};

#endif
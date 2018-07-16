#ifndef _UNIVERSE_HPP
#define _UNIVERSE_HPP

class Klass;
class HiInteger;
class HiObject;

class Universe {
public:
    static Klass* object_klass;
    static Klass* type_klass;
    static Klass* int_klass;

    static HiInteger* HiTrue;
    static HiInteger* HiFalse;

    static HiObject* HiNone;

public:
    static void genesis();
};

#endif

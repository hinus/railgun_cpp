#ifndef _UNIVERSE_HPP
#define _UNIVERSE_HPP

class Klass;
class HiInteger;
class HiObject;

class Universe {
public:
    static Klass* _object_klass;
    static Klass* _type_klass;
    static Klass* _int_klass;

    static HiInteger* HiTrue;
    static HiInteger* HiFalse;

	static HiObject* HiNone;

public:
    static void genesis();
};

#endif

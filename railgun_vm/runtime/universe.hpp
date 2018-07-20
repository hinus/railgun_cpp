#ifndef _UNIVERSE_HPP
#define _UNIVERSE_HPP

class Klass;
class HiInteger;
class HiObject;
class Heap;
class CodeObject;
class OopClosure;

template <typename T>
class ArrayList;

class Universe {
public:
    static Klass* object_klass;
    static Klass* type_klass;
    static Klass* int_klass;

    static HiInteger* HiTrue;
    static HiInteger* HiFalse;

    static HiObject* HiNone;
    static Heap* heap;

    static CodeObject* main_code;
    static ArrayList<Klass*>* klasses;

public:
    static void genesis();
    static void destroy();
    static void oops_do(OopClosure* closure);
};

#endif

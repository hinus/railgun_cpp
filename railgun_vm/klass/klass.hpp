#ifndef _KLASS_HPP
#define _KLASS_HPP

#include <stdio.h>

enum vm_type {
    TYPE_INT = 0,
    TYPE_STRING,
    TYPE_OBJECT,
    TYPE_TYPE,
};

class HiTypeObject;
class HiObject;

class Klass {
private:
    Klass* _super;
    HiTypeObject* _type_object;

public:
    static Klass* allocate_instance_klass();

    void set_super(Klass* x)              { _super = x; }
    void set_type_object(HiTypeObject* x) { _type_object = x; }

    virtual void print(HiObject* obj) {}

	virtual HiObject* add(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* sub(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* mul(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* div(HiObject* x, HiObject* y) { return 0; }

    // alloacte instances
    virtual HiObject* allocate_instance() { return 0; }
};

#endif

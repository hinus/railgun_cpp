#ifndef _KLASS_HPP
#define _KLASS_HPP

enum vm_type {
    TYPE_INT = 0,
    TYPE_STRING,
    TYPE_OBJECT,
    TYPE_TYPE,
}

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

    virtual HiObject* add(HiObject* x, HiObject* y) {}
    virtual HiObject* sub(HiObject* x, HiObject* y) {}
    virtual HiObject* mul(HiObject* x, HiObject* y) {}
    virtual HiObject* div(HiObject* x, HiObject* y) {}

    // alloacte instances
    virtual HiObject* allocate_instance() {}
};

#endif

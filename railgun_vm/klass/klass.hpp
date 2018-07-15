#ifndef _KLASS_HPP
#define _KLASS_HPP

#include <stdio.h>
#include "util/map.hpp"
#include "util/arrayList.hpp"

enum vm_type {
    TYPE_INT = 0,
    TYPE_STRING,
    TYPE_OBJECT,
    TYPE_TYPE,
};

class HiTypeObject;
class HiObject;
class HiString;
class NativeFunction;

typedef Map<HiObject*, HiObject*>* InnerDict;

class Klass {
private:
    Klass* _super;
    HiTypeObject* _type_object;
    InnerDict _klass_dict;

public:
    static Klass* allocate_instance_klass();

    void set_super(Klass* x)              { _super = x; }
    void set_type_object(HiTypeObject* x) { _type_object = x; }
    HiTypeObject* type_object()           { return _type_object; }

    void set_klass_dict(InnerDict dict)   { _klass_dict = dict; }
    InnerDict klass_dict()                { return _klass_dict; }

    virtual void print(HiObject* obj) {}

    virtual HiObject* greater  (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* less     (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* equal    (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* not_equal(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* ge       (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* le       (HiObject* x, HiObject* y) { return 0; }

	virtual HiObject* add(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* sub(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* mul(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* div(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* mod(HiObject* x, HiObject* y) { return 0; }

    virtual HiObject* getattr(HiObject* x, HiString* y) { return 0; }

    // alloacte instances
    virtual HiObject* allocate_instance() { return 0; }
	virtual HiObject* call(ArrayList<HiObject*>* args) { return 0; }
};

#endif

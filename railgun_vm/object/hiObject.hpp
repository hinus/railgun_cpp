#ifndef _HI_OBJECT_HPP
#define _HI_OBJECT_HPP

#include "klass/klass.hpp"
#include <assert.h>

class HiString;

class ObjectKlass : public Klass {
private:
    ObjectKlass();
    static ObjectKlass* instance;

public:
    static ObjectKlass* get_instance();
};

class HiObject {
private:
    long    _mark_word;
    Klass*  _klass;
    HiDict* _obj_dict;

public:
    HiObject() {
        _mark_word = 0;
        _klass = ObjectKlass::get_instance();
        _obj_dict = NULL; 
    }

    Klass* klass()             { assert(_klass != NULL); return _klass; }
    void set_klass(Klass* x)   { _klass = x; }

    HiDict* obj_dict()             { return _obj_dict; }
    HiObject** obj_dict_address()  { return (HiObject**)&_obj_dict; }

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

    HiObject* bi_and(HiObject* x);

    HiObject* iter();
    HiObject* next();

    HiObject* getattr(HiObject* x);
    HiObject* setattr(HiObject* x, HiObject* y);
    void      delattr(HiObject* x);
    HiObject* subscr(HiObject* x);
    void      store_subscr(HiObject* x, HiObject* y); 

    void* operator new(size_t size);

    // interfaces for GC.
    void oops_do(OopClosure* closure);
    size_t size();
    char* new_address();
    void set_new_address(char* addr);
};

/*
 * meta-klass for the object system.
 */

class TypeKlass : public Klass {
private:
    TypeKlass() {}
    static TypeKlass* instance;

public:
    static TypeKlass* get_instance();

    virtual void print(HiObject* obj);
    virtual size_t size();
    virtual void oops_do(OopClosure* f, HiObject* obj);
};

class HiTypeObject : public HiObject {
private:
    Klass*  _own_klass;

public:
    HiTypeObject();

    void    set_own_klass(Klass* k);
    Klass*  own_klass()             { return _own_klass; }
};

#endif

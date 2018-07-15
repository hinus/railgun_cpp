#ifndef _HI_OBJECT_HPP
#define _HI_OBJECT_HPP

#include "klass/klass.hpp"
#include <assert.h>

class HiString;

class HiObject {
private:
    long _mark_word;
    Klass* _klass;
    Map<HiObject*, HiObject*>* _obj_dict;

public:
    HiObject() {
        _mark_word = 0;
        _klass = 0;
        _obj_dict = new Map<HiObject*, HiObject*>();
    }

	~HiObject() {
		delete _obj_dict;
	}

    Klass* klass()             { assert(_klass != NULL); return _klass; }
    void set_klass(Klass* x)   { _klass = x; }

    Map<HiObject*, HiObject*>* obj_dict()  { return _obj_dict; }

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

    HiObject* getattr(HiObject* x);
};

class HiTypeObject : public HiObject {
};

#endif

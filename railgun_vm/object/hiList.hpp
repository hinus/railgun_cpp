#ifndef HI_LIST_HPP
#define HI_LIST_HPP

#include "klass/klass.hpp"
#include "object/hiString.hpp"
#include "object/hiObject.hpp"
#include "runtime/nativeFunction.hpp"
#include "util/arrayList.hpp"

class ListKlass : public Klass {
private:
	ListKlass();
	static ListKlass* instance;

public:
	static ListKlass* get_instance();

    virtual HiObject* getattr(HiObject* x, HiString* y);
	virtual void print(HiObject* obj);
};

class HiList : public HiObject {
friend class ListAppendMethod;

friend class ListKlass;

private:
    ArrayList<HiObject*>* _inner_list;
public:
    HiList();
    ArrayList<HiObject*>* inner_list()  { return _inner_list; }
};

class ListAppendMethod : public NativeFunction {
public:
    ListAppendMethod() : NativeFunction("append") {};
    virtual HiObject* operator () (HiObject* obj, ArgsList args);
};

#endif

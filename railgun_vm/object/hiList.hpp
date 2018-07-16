#ifndef HI_LIST_HPP
#define HI_LIST_HPP

#include "klass/klass.hpp"
#include "object/hiString.hpp"
#include "object/hiObject.hpp"
#include "runtime/functionObject.hpp"
#include "util/arrayList.hpp"

class ListKlass : public Klass {
private:
	ListKlass();
	static ListKlass* instance;

public:
	static ListKlass* get_instance();

	virtual void print(HiObject* obj);
};

class HiList : public HiObject {
friend class ListKlass;

private:
    ArrayList<HiObject*>* _inner_list;
public:
    HiList();
    HiList(ObjList ol);
    ArrayList<HiObject*>* inner_list()  { return _inner_list; }
};

class ListAppendKlass : public Klass {
private:
	ListAppendKlass() {
		set_super(NativeFunctionKlass::get_instance());
	};

	static ListAppendKlass* instance;

public:
	static ListAppendKlass* get_instance();
    
public:
    virtual HiObject* call(ObjList args);
};

#endif

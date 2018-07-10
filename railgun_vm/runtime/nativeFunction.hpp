#ifndef NATIVE_FUNCTION_HPP
#define NATIVE_FUNCTION_HPP

#include "object/hiObject.hpp"
#include "object/hiString.hpp"
#include "util/arrayList.hpp"

class NativeFunctionKlass : public Klass {
private:
	NativeFunctionKlass() {}
	static NativeFunctionKlass* instance;

public:
	static NativeFunctionKlass* get_instance();

	virtual void print(HiObject* obj);
};

typedef ArrayList<HiObject*>* ArgsList;

class NativeFunction : public HiObject {
friend class NativeFunctionKlass;

private:
    HiString* _func_name;

public:
    NativeFunction(const char* name);    
    NativeFunction(HiString* name);    

    virtual HiObject* operator () (HiObject* obj, ArgsList args) { return 0; }

    bool equals(HiString* x);
};

#endif

#ifndef HI_FUNCTION_OBJECT_HPP
#define HI_FUNCTION_OBJECT_HPP

#include "code/codeObject.hpp"
#include "klass/klass.hpp"

typedef ArrayList<HiObject*>* ArgsList;

class FunctionKlass : public Klass {
private:
	FunctionKlass();
	static FunctionKlass* instance;

public:
	static FunctionKlass* get_instance();

	virtual void print(HiObject* obj);
};

class FunctionObject : public HiObject {
friend class Interpreter;
friend class FrameObject;

private:
    CodeObject* _func_code;
    HiString*   _func_name;
    ArrayList<HiObject*>* _globals;
    ArrayList<HiObject*>* _defaults;

public:
    FunctionObject(HiObject* code_object, ArrayList<HiObject*>* globals);
    FunctionObject(Klass* klass) {
        _func_code = NULL;
        _func_name = NULL;
        _globals   = NULL;
        _defaults  = NULL;

        set_klass(klass);
    }
    
    void set_default(ArgsList defaults);

	HiString*  func_name()   { return _func_name; }
	HiObject*  call(ArgsList args)        { return klass()->call(args); }
};


// Method objects.
class MethodKlass : public Klass {
private:
	MethodKlass();
	static MethodKlass* instance;

public:
	static MethodKlass* get_instance();
};

class MethodObject : public HiObject {
private:
    HiObject* _owner;
    FunctionObject* _func;

public:
    MethodObject(FunctionObject* func) : _func(func), _owner(NULL) {
        set_klass(MethodKlass::get_instance());
    }

    MethodObject(FunctionObject* func, HiObject* owner) : _func(func), _owner(owner) {
        set_klass(MethodKlass::get_instance());
    }

    void set_owner(HiObject * x)   { _owner = x; }
    HiObject* owner()              { return _owner; }
    FunctionObject* func()         { return _func; }

	static bool is_native(HiObject* x);
	static bool is_method(HiObject* x);
	static bool is_function(HiObject* x);
};

class NativeFunctionKlass : public Klass {
private:
	NativeFunctionKlass();
	static NativeFunctionKlass* instance;

public:
	static NativeFunctionKlass* get_instance();
};

#endif

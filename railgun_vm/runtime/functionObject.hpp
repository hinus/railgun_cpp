#ifndef HI_FUNCTION_OBJECT_HPP
#define HI_FUNCTION_OBJECT_HPP

#include "code/codeObject.hpp"
#include "klass/klass.hpp"

class OopClosure;
class HiDict;
class Generator;

class FunctionKlass : public Klass {
private:
    FunctionKlass();
    static FunctionKlass* instance;

public:
    static FunctionKlass* get_instance();

    virtual void print(HiObject* obj);
    virtual size_t size();
    virtual void oops_do(OopClosure* f, HiObject* obj);
};

class FunctionObject : public HiObject {
friend class FunctionKlass;
friend class FrameObject;
friend class Generator;

private:
    CodeObject* _func_code;
    HiString*   _func_name;
    HiDict*     _globals;
    ObjList     _defaults;

    unsigned int _flags;

public:
    FunctionObject(HiObject* code_object, HiDict* globals);
    FunctionObject(Klass* klass) {
        _func_code = NULL;
        _func_name = NULL;
        _globals   = NULL;
        _defaults  = NULL;
        _flags     = 0;

        set_klass(klass);
    }
    
    void set_default(ObjList defaults);

    HiString*  func_name()   { return _func_name; }
    HiObject*  call(ObjList args)        { return klass()->call(args); }
    int  flags()             { return _flags; }
};

// Method objects.
class MethodKlass : public Klass {
private:
    MethodKlass();
    static MethodKlass* instance;

public:
    static MethodKlass* get_instance();

    virtual size_t size();
    virtual void oops_do(OopClosure* f, HiObject* obj);
};

class MethodObject : public HiObject {
friend class MethodKlass;

private:
    HiObject* _owner;
    FunctionObject* _func;

public:
    MethodObject(FunctionObject* func) : _owner(NULL), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    MethodObject(FunctionObject* func, HiObject* owner) : _owner(owner), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    void set_owner(HiObject * x)   { _owner = x; }
    HiObject* owner()              { return _owner; }
    FunctionObject* func()         { return _func; }

    static bool is_native(HiObject* x);
    static bool is_method(HiObject* x);
    static bool is_function(HiObject* x);
    static bool is_yield_function(HiObject* x);
};

class NativeFunctionKlass : public Klass {
private:
    NativeFunctionKlass();
    static NativeFunctionKlass* instance;

public:
    static NativeFunctionKlass* get_instance();
};

class SysGCKlass : public Klass {
private:
    SysGCKlass() ;
    static SysGCKlass* instance;

public:
    static SysGCKlass* get_instance();
    
public:
    virtual HiObject* call(ObjList args);
};

#endif

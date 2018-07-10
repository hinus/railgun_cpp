#ifndef HI_FUNCTION_OBJECT_HPP
#define HI_FUNCTION_OBJECT_HPP

#include "code/codeObject.hpp"

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
    
    void set_default(ArrayList<HiObject*>* defaults);
};

class NativeFunction;

class MethodObject : public HiObject {
private:
    HiObject* _owner;
    FunctionObject* _func_ref;
    NativeFunction* _nfunc_ref;

public:
    MethodObject(HiObject* owner, FunctionObject* func, NativeFunction* nfunc) {
        _owner = owner;
        _func_ref = func;
        _nfunc_ref = nfunc;
    }
    
    HiObject* owner()               { return _owner; }
    FunctionObject* func()          { return _func_ref; }
    NativeFunction* native_func()   { return _nfunc_ref; }
};

#endif

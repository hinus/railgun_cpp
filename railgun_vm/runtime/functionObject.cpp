#include "runtime/functionObject.hpp"

FunctionKlass* FunctionKlass::instance = NULL;

FunctionKlass* FunctionKlass::get_instance() {
    if (instance == NULL)
        instance = new FunctionKlass();

    return instance;
}

FunctionKlass::FunctionKlass() {
    set_klass_dict(new Map<HiObject*, HiObject*>());
    HiTypeObject* function_type_obj = new HiTypeObject();
    set_type_object(function_type_obj);
    set_super(NULL);
}

FunctionObject::FunctionObject(HiObject* code_object, 
    NameTable globals) {
    CodeObject* co = (CodeObject*) code_object;

    _func_code = co;
    _func_name = co->_co_name;
    _globals   = globals;

    set_klass(FunctionKlass::get_instance());
}

void FunctionObject::set_default(ArrayList<HiObject*>* defaults) {
    if (defaults == NULL) {
        _defaults = NULL;
        return;
    }

    _defaults = new ArrayList<HiObject*>(defaults->length());

    for (int i = 0; i < defaults->length(); i++) {
        _defaults->set(i, defaults->get(i));
    }
}

void FunctionKlass::print(HiObject* obj) {
    printf("native function : ");
    FunctionObject* fo = static_cast<FunctionObject*>(obj);

    assert(fo && fo->klass() == (Klass*) this);
    fo->func_name()->print();
}

/*
 *  Operations for methods
 *  Method is a wrapper for function.
 */

MethodKlass* MethodKlass::instance = NULL;

MethodKlass* MethodKlass::get_instance() {
    if (instance == NULL)
        instance = new MethodKlass();

    return instance;
}

MethodKlass::MethodKlass() {
    set_klass_dict(new Map<HiObject*, HiObject*>());

    HiTypeObject* type_obj = new HiTypeObject();
    set_type_object(type_obj);
    set_super(FunctionKlass::get_instance());
}

/*
 * To check the type of a callable object.
 */
bool MethodObject::is_native(HiObject *x) {
    Klass* k = x->klass();
    if (k == (Klass*) NativeFunctionKlass::get_instance())
        return true;

    while (k->super() != NULL) {
        k = k->super();
        if (k == (Klass*) NativeFunctionKlass::get_instance())
            return true;
    }
    return false;
}

bool MethodObject::is_method(HiObject *x) {
    if (x->klass() == (Klass*) MethodKlass::get_instance())
        return true;

    return false;
}

bool MethodObject::is_function(HiObject *x) {
    Klass* k = x->klass();
    if (k == (Klass*) FunctionKlass::get_instance())
        return true;

    while (k->super() != NULL) {
        k = k->super();
        if (k == (Klass*) FunctionKlass::get_instance())
            return true;
    }

    return false;
}

NativeFunctionKlass* NativeFunctionKlass::instance = NULL;

NativeFunctionKlass* NativeFunctionKlass::get_instance() {
    if (instance == NULL)
        instance = new NativeFunctionKlass();

    return instance;
}

NativeFunctionKlass::NativeFunctionKlass() {
    set_super(FunctionKlass::get_instance());
}


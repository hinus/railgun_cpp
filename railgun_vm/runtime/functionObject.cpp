#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"

FunctionKlass* FunctionKlass::instance = NULL;

FunctionKlass* FunctionKlass::get_instance() {
    if (instance == NULL)
        instance = new FunctionKlass();

    return instance;
}

FunctionKlass::FunctionKlass() {
    set_klass_dict(new HiDict());
    HiTypeObject* function_type_obj = new HiTypeObject();
    set_type_object(function_type_obj);
    set_super(NULL);
}

FunctionObject::FunctionObject(HiObject* code_object, 
    HiDict* globals) {
    CodeObject* co = (CodeObject*) code_object;

    _func_code = co;
    _func_name = co->_co_name;
    _globals   = globals;
    _flags     = co->_flag;

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

size_t FunctionKlass::size() {
    return sizeof(FunctionObject);
}

void FunctionKlass::oops_do(OopClosure* f, HiObject* obj) {
    FunctionObject* fo = (FunctionObject*)obj;
    assert(fo->klass() == (Klass*)this);

    f->do_oop((HiObject**)&fo->_func_code);
    f->do_oop((HiObject**)&fo->_func_name);
    f->do_oop((HiObject**)&fo->_globals);
    f->do_array_list(&fo->_defaults);
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
    set_klass_dict(new HiDict());

    HiTypeObject* type_obj = new HiTypeObject();
    set_type_object(type_obj);
    set_super(FunctionKlass::get_instance());
}

size_t MethodKlass::size() {
    return sizeof(MethodObject);
}

void MethodKlass::oops_do(OopClosure* f, HiObject* obj) {
    MethodObject* mo = (MethodObject*)obj;
    assert(mo->klass() == (Klass*)this);

    f->do_oop((HiObject**)&mo->_owner);
    f->do_oop((HiObject**)&mo->_func);
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

bool MethodObject::is_yield_function(HiObject *x) {
    Klass* k = x->klass();
    if (k != (Klass*) FunctionKlass::get_instance())
        return false;

    FunctionObject* fo = (FunctionObject*)x;
    return ((fo->flags() & 0x20) != 0);
}
/*
 * Operations for native calls.
 */

NativeFunctionKlass* NativeFunctionKlass::instance = NULL;

NativeFunctionKlass* NativeFunctionKlass::get_instance() {
    if (instance == NULL)
        instance = new NativeFunctionKlass();

    return instance;
}

NativeFunctionKlass::NativeFunctionKlass() {
    set_super(FunctionKlass::get_instance());
}

SysGCKlass* SysGCKlass::instance = NULL;

SysGCKlass* SysGCKlass::get_instance() {
    if (instance == NULL)
        instance = new SysGCKlass();

    return instance;
}

SysGCKlass::SysGCKlass() {
    set_super(NativeFunctionKlass::get_instance());
}

HiObject* SysGCKlass::call(ObjList args) {
    Universe::heap->gc();
    return NULL;
}


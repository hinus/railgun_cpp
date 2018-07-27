#include "memory/oopClosure.hpp"
#include "memory/heap.hpp"
#include "klass/klass.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "code/interpreter.hpp"
#include "object/hiObject.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/hiString.hpp"
#include "util/debug.hpp"

#define ST(x) StringTable::get_instance()->STR(x)
#define STR(x) x##_str

Klass::Klass() {
    Universe::klasses->add(this);
    _klass_dict = NULL;
    _name = NULL;
}

HiObject* Klass::create_klass(HiObject* x, HiObject* supers, HiObject* name) {
    assert(x->klass()      == (Klass*)DictKlass::get_instance());
    assert(supers->klass() == (Klass*)ListKlass::get_instance());
    assert(name->klass()   == (Klass*)StringKlass::get_instance());

    Klass* new_klass   = new Klass();
    HiDict* klass_dict = (HiDict*) x;
    HiList* supers_list = (HiList*) supers;
    new_klass->set_klass_dict(klass_dict);
    new_klass->set_name((HiString*)name);
    if (supers_list->inner_list()->length() > 0) {
        HiTypeObject* super = (HiTypeObject*)supers_list->inner_list()->get(0);
        new_klass->set_super(super->own_klass());
    }

    HiTypeObject* type_obj = new HiTypeObject();
    type_obj->set_own_klass(new_klass);
    
    return type_obj;
}

void* Klass::operator new(size_t size) {
    return Universe::heap->allocate_meta(size);
}

// this function will visit all children
void Klass::oops_do(OopClosure* closure, HiObject* obj) {
    closure->do_oop(obj->obj_dict_address());
}

void Klass::oops_do(OopClosure* f) {
    f->do_oop((HiObject**)&_name);
    f->do_oop((HiObject**)&_klass_dict);
    f->do_oop((HiObject**)&_type_object);
}

size_t Klass::size() {
    return sizeof(HiObject);
}

HiObject* Klass::add(HiObject* lhs, HiObject* rhs) {
    ObjList args = new ArrayList<HiObject*>();
    args->add(rhs);
    return find_and_call(lhs, args, ST(add));
}

HiObject* Klass::sub(HiObject* lhs, HiObject* rhs) {
    ObjList args = new ArrayList<HiObject*>();
    args->add(rhs);
    return find_and_call(lhs, args, ST(sub));
}

HiObject* Klass::bi_and(HiObject* lhs, HiObject* rhs) {
    ObjList args = new ArrayList<HiObject*>();
    args->add(rhs);
    return find_and_call(lhs, args, ST(and));
}

HiObject* Klass::find_and_call(HiObject* lhs, ObjList args, HiObject* func_name) {
    HiObject* func = lhs->getattr(func_name);
    if (func != Universe::HiNone) {
        if (!args)
            args = new ArrayList<HiObject*>();

        return Interpreter::get_instance()->call_virtual(func, args);
    }

    printf("class ");
    lhs->klass()->name()->print();
    Debug::fatal(" Error : unsupport operation for class ");
    return Universe::HiNone;
}

HiObject* Klass::subscr(HiObject* x, HiObject* y) {
    ObjList args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(getitem));
}

void Klass::store_subscr(HiObject* x, HiObject* y, HiObject* z) {
    ObjList args = new ArrayList<HiObject*>();
    args->add(y);
    args->add(z);
    find_and_call(x, args, ST(setitem));
}

void Klass::print(HiObject* x) {
    printf("instance of ");
    x->klass()->name()->print();
}

HiObject* Klass::iter(HiObject* x) {
    return find_and_call(x, NULL, ST(iter));
}

HiObject* Klass::next(HiObject* x) {
    return find_and_call(x, NULL, ST(next));
}

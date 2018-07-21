#include "memory/oopClosure.hpp"
#include "memory/heap.hpp"
#include "klass/klass.hpp"
#include "runtime/universe.hpp"
#include "object/hiObject.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/hiString.hpp"

#include <assert.h>

Klass::Klass() {
    Universe::klasses->add(this);
}

HiObject* Klass::create_klass(HiObject* x, HiObject* supers, HiObject* name) {
    assert(x->klass()      == (Klass*)DictKlass::get_instance());
    assert(supers->klass() == (Klass*)ListKlass::get_instance());
    assert(name->klass()   == (Klass*)StringKlass::get_instance());

    Klass* new_klass   = new Klass();
    HiDict* klass_dict = (HiDict*) x;
    HiList* supers_list = (HiList*) supers;
    new_klass->set_klass_dict(klass_dict->map());
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
    closure->do_map(obj->obj_dict_address());
}

void Klass::oops_do(OopClosure* f) {
    f->do_oop((HiObject**)&_name);
    f->do_map(&_klass_dict);
    f->do_map(_type_object->obj_dict_address());
}

size_t Klass::size() {
    return sizeof(HiObject);
}

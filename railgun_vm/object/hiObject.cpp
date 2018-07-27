#include "object/hiObject.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/universe.hpp"
#include "memory/oopClosure.hpp"
#include "memory/heap.hpp"
#include <assert.h>

ObjectKlass* ObjectKlass::instance = NULL;

ObjectKlass::ObjectKlass() {
    set_super(NULL);
}

ObjectKlass* ObjectKlass::get_instance() {
    if (instance == NULL)
        instance = new ObjectKlass();

    return instance;
}

void HiObject::print() {
    klass()->print(this);
}

HiObject* HiObject::greater(HiObject * rhs) {
    return klass()->greater(this, rhs);
}

HiObject* HiObject::add(HiObject * rhs) {
    assert(_klass != NULL);

    return _klass->add(this, rhs);
}

HiObject* HiObject::sub(HiObject * rhs) {
    assert(_klass != NULL);

    return _klass->sub(this, rhs);
}

HiObject* HiObject::mul(HiObject * rhs) {
    assert(_klass != NULL);

    return _klass->mul(this, rhs);
}

HiObject* HiObject::div(HiObject * rhs) {
    assert(_klass != NULL);

    return _klass->div(this, rhs);
}

HiObject* HiObject::mod(HiObject * rhs) {
    assert(_klass != NULL);

    return _klass->mod(this, rhs);
}

HiObject* HiObject::bi_and(HiObject * rhs) {
    return klass()->bi_and(this, rhs);
}

HiObject* HiObject::less(HiObject * rhs) {
    assert(_klass != NULL);

    return _klass->less(this, rhs);
}

HiObject* HiObject::equal(HiObject * rhs) {
    assert(_klass != NULL);

    return _klass->equal(this, rhs);
}

HiObject* HiObject::not_equal(HiObject * rhs) {
    assert(_klass != NULL);

    return _klass->not_equal(this, rhs);
}

HiObject* HiObject::ge(HiObject * rhs) {
    return klass()->ge(this, rhs);
}

HiObject* HiObject::le(HiObject * rhs) {
    return klass()->le(this, rhs);
}

HiObject* HiObject::getattr(HiObject* x) {
    HiObject* result = Universe::HiNone;

    if (_obj_dict)
        result = _obj_dict->get(x);

    if (result != Universe::HiNone)
        return result;

    HiDict* attr_dict = klass()->type_object()->obj_dict();
    if (attr_dict)
        result = attr_dict->get(x);

    if (result != Universe::HiNone)
        return result;

    if (!klass()->klass_dict())
        return Universe::HiNone;

    result = klass()->klass_dict()->get(x);

    if (result == Universe::HiNone)
        return Universe::HiNone;

    // Only klass attribute needs bind.
    if (MethodObject::is_function(result)) {
        result = new MethodObject((FunctionObject*)result, this);
    }
    return result;
}

HiObject* HiObject::setattr(HiObject* x, HiObject* y) {
    if (!_obj_dict)
        _obj_dict = new HiDict();

    _obj_dict->put(x, y);
    return Universe::HiNone;
}

void HiObject::delattr(HiObject* x) {
    if (!_obj_dict)
        return;
    
    _obj_dict->remove(x);
}

HiObject* HiObject::subscr(HiObject* x) {
    return klass()->subscr(this, x);
}

void HiObject::store_subscr(HiObject* x, HiObject* y) {
    klass()->store_subscr(this, x, y);
}

HiObject* HiObject::iter() {
    return klass()->iter(this);
}

HiObject* HiObject::next() {
    return klass()->next(this);
}

/*
 * Interfaces for GC.
 */
void HiObject::oops_do(OopClosure* closure) {
    // object does not know who to visit, klass knows
    klass()->oops_do(closure, this);
}

size_t HiObject::size() {
    return klass()->size();
}

char* HiObject::new_address() {
    if ((_mark_word & 0x1) == 0x1)
        return (char*)(_mark_word & ((long)-8));

    return NULL;
}

void HiObject::set_new_address(char* addr) {
    if (!addr)
        return;

    _mark_word = ((long)addr) | 0x1;
}

void* HiObject::operator new(size_t size) {
    return Universe::heap->allocate(size);
}

/*
 * TypeObject is a special object
 */
TypeKlass* TypeKlass::instance = NULL;

TypeKlass* TypeKlass::get_instance() {
    if (instance == NULL)
        instance = new TypeKlass();

    return instance;
}

void TypeKlass::print(HiObject* obj) {
    assert(obj->klass() == (Klass*) this);
    printf("<type ");
    Klass* own_klass = ((HiTypeObject*)obj)->own_klass();

    HiDict* attr_dict = own_klass->klass_dict();
    if (attr_dict) {
        HiObject* mod = attr_dict->get(new HiString("__module__"));
        if (mod) {
            mod->print();
            printf(".");
        }
    }

    own_klass->name()->print();
    printf(">");
}

size_t TypeKlass::size() {
    return sizeof(HiTypeObject);
}

void TypeKlass::oops_do(OopClosure* f, HiObject* obj) {
    // do nothing since HiTypeObject refers no oop.
}

HiTypeObject::HiTypeObject() {
    set_klass(TypeKlass::get_instance());
}

void HiTypeObject::set_own_klass(Klass* k) {
    _own_klass = k; 
    k->set_type_object(this);
}


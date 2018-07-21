#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "runtime/universe.hpp"
#include <assert.h>

StringKlass* StringKlass::instance = NULL;

StringKlass* StringKlass::get_instance() {
    if (instance == NULL)
        instance = new StringKlass();

    return instance;
}

HiObject* StringKlass::equal(HiObject* x, HiObject* y) {
    if (x->klass() != y->klass())
        return Universe::HiFalse;

    HiString* sx = (HiString*) x;
    HiString* sy = (HiString*) y;

    assert(sx && sx->klass() == (Klass*)this);
    assert(sy && sy->klass() == (Klass*)this);

    if (sx->length() != sy->length())
        return Universe::HiFalse;

    for (int i = 0; i < sx->length(); i++) {
        if (sx->value()[i] != sy->value()[i])
            return Universe::HiFalse;
    }

    return Universe::HiTrue;
}

void StringKlass::print(HiObject* obj) {
    HiString* str_obj = (HiString*) obj;
    assert(str_obj && str_obj->klass() == (Klass*)this);

    for (int i = 0; i < str_obj->length(); i++) {
        printf("%c", str_obj->value()[i]);
    }
}

void StringKlass::oops_do(OopClosure* closure, HiObject* obj) {
    HiString* str_obj = (HiString*) obj;
    assert(str_obj && str_obj->klass() == (Klass*)this);

    closure->do_map(str_obj->obj_dict_address());
    closure->do_raw_mem(str_obj->value_address(), str_obj->length());
}

size_t StringKlass::size() {
    return sizeof(HiString);
}

HiString::HiString(const char* x) {
    _length = strlen(x);
    _value = (char*)Universe::heap->allocate(_length);
    strcpy(_value, x);
    set_klass(StringKlass::get_instance());
}

HiString::HiString(const char * x, const int length) {
    _length = length;
    _value = (char*)Universe::heap->allocate(_length);

    // do not use strcpy here, since '\0' is allowed.
    for (int i = 0; i < length; i++) {
        _value[i] = x[i];
    }

    set_klass(StringKlass::get_instance());
}

void* HiString::operator new(size_t size) {
    return Universe::heap->allocate(size);
}


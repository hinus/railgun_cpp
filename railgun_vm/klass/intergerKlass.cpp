#include <iostream>

#include "klass/integerKlass.hpp"
#include "Object/hiObject.hpp"
#include "Object/hiInteger.hpp"

void IntegerKlass::print(HiObject* obj) {
    printf("%d", ((HiInteger*)obj)->value());
}

HiObject* IntegerKlass::add(HiObject* x, HiObject* y) {
    return new HiInteger(((HiInteger*)x)->value() + ((HiInteger*)y)->value());
}

HiObject* IntegerKlass::sub(HiObject* x, HiObject* y) {
    return new HiInteger(((HiInteger*)x)->value() - ((HiInteger*)y)->value());
}

HiObject* IntegerKlass::mul(HiObject* x, HiObject* y) {
    return new HiInteger(((HiInteger*)x)->value() * ((HiInteger*)y)->value());
}

HiObject* IntegerKlass::div(HiObject* x, HiObject* y) {
    return new HiInteger(((HiInteger*)x)->value() / ((HiInteger*)y)->value());
}

// we use new HiInteger instead of this method.
HiObject* IntegerKlass::allocate_instance() {
    return NULL;
}

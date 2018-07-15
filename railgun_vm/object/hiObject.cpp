#include "object/hiObject.hpp"
#include "runtime/functionObject.hpp"
#include <assert.h>

void HiObject::print() {
    assert(_klass != NULL);

    _klass->print(this);
}

HiObject* HiObject::greater(HiObject * rhs) {
    assert(_klass != NULL);

    return _klass->greater(this, rhs);
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
    HiObject* result = obj_dict()->get(x);

    if (result)
        return result;

    result = klass()->type_object()->obj_dict()->get(x);

    if (result)
        return result;

    result = klass()->klass_dict()->get(x);

    if (MethodObject::is_function(result)) {
        result = new MethodObject((FunctionObject*)result, this);
    }
    return result;
}


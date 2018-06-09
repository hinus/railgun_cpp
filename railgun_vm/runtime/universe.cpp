#include "VirtualMachine/universe.hpp"
#include "klass/objectKlass.hpp"
#include "klass/typeKlass.hpp"

void Universe::genesis() {
    TypeKlass* tp_klass = (TypeKlass *)Klass::allocate_instance_klass(TYPE_TYPE);
    ObjectKlass* obj_klass = (ObjectKlass *)Klass::allocate_instance_klass(TYPE_OBJECT);

    tp_klass->set_type_object((HiTypeObject*)tp_klass->allocate_instance());

    obj_klass->set_super(NULL);
    obj_klass->set_type_object((HiTypeObject*)tp_klass->allocate_instance());

    Universe::_object_klass = obj_klass;
    Universe::_type_klass = tp_klass;
    Universe::_type_klass = tp_klass;
}

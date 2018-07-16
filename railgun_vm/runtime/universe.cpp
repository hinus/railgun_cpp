#include "runtime/universe.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiObject.hpp"

Klass* Universe::type_klass = TypeKlass::get_instance();
Klass* Universe::int_klass  = IntegerKlass::get_instance();

void Universe::genesis() {
    HiTypeObject* tp_obj = new HiTypeObject();
    type_klass->set_name(new HiString("'type'"));
    type_klass->set_type_object(tp_obj);

    int_klass->set_name(new HiString("'int'"));
    //obj_klass->set_type_object((HiTypeObject*)type_klass->allocate_instance());
}

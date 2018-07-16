#include "runtime/universe.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiObject.hpp"

Klass* Universe::object_klass  = ObjectKlass::get_instance();
Klass* Universe::type_klass = TypeKlass::get_instance();
Klass* Universe::int_klass  = IntegerKlass::get_instance();

void Universe::genesis() {
    HiTypeObject* tp_obj = new HiTypeObject();
    type_klass->set_name(new HiString("'type'"));
    tp_obj->set_own_klass(type_klass);
    type_klass->set_super(object_klass);

    int_klass->set_name(new HiString("'int'"));
    int_klass->set_super(object_klass);
    
    object_klass->set_name(new HiString("'object'"));
    HiTypeObject* obj_obj = new HiTypeObject();
    obj_obj->set_own_klass(object_klass);
    object_klass->set_super(NULL);
}

#include "util/arrayList.hpp"
#include "runtime/universe.hpp"
#include "code/codeObject.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiObject.hpp"
#include "object/hiNoneObject.hpp"
#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"

Klass* Universe::object_klass = NULL;
Klass* Universe::type_klass   = NULL;
Klass* Universe::int_klass    = NULL;

KlassList Universe::klasses   = NULL;

Heap* Universe::heap          = NULL;
HiInteger* Universe::HiTrue   = NULL;
HiInteger* Universe::HiFalse  = NULL;
HiObject*  Universe::HiNone   = NULL;

CodeObject* Universe::main_code = NULL;

void Universe::genesis() {
    heap = Heap::get_instance();

    klasses = new ArrayList<Klass*>();

    object_klass = ObjectKlass::get_instance();
    type_klass   = TypeKlass::get_instance();
    int_klass    = IntegerKlass::get_instance();

    HiTrue       = new HiInteger(1);
    HiFalse      = new HiInteger(0);
    HiNone       = HiNoneObject::get_instance();

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

void Universe::destroy() {
    if (heap)
        delete heap;
}

void Universe::oops_do(OopClosure* closure) {
    closure->do_klass(&int_klass);
    closure->do_klass(&type_klass);
    closure->do_klass(&object_klass);

    closure->do_oop((HiObject**)&HiTrue);
    closure->do_oop((HiObject**)&HiFalse);
    closure->do_oop((HiObject**)&HiNone);
    closure->do_oop((HiObject**)&main_code);

    closure->do_array_list(&klasses);
}


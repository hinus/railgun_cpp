#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "runtime/universe.hpp"
#include <assert.h>

ListKlass* ListKlass::instance = NULL;

ListKlass* ListKlass::get_instance() {
    if (instance == NULL)
        instance = new ListKlass();

    return instance;
}

ListKlass::ListKlass() {
    set_klass_dict(new Map<HiObject*, HiObject*>());
    klass_dict()->put(new HiString("append"), 
        new FunctionObject(ListAppendKlass::get_instance()));

    HiTypeObject* list_type_obj = new HiTypeObject();
    set_type_object(list_type_obj);
}

void ListKlass::print(HiObject* x) {
    HiList * lx = (HiList*)x;
    assert(lx && lx->klass() == (Klass*) this);

    printf("[");

    int size = lx->_inner_list->size();
    if (size >= 1)
        lx->_inner_list->get(0)->print();

    for (int i = 1; i < size; i++) {
        printf(", ");
        lx->_inner_list->get(i)->print();
    }
    printf("]");
}

HiObject* ListKlass::subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == (Klass*) this);
    assert(y && y->klass() == (Klass*) Universe::int_klass);

    HiList * lx = (HiList*)x;
    HiInteger* iy = (HiInteger*)y;
    
    return lx->inner_list()->get(iy->value());
}

HiList::HiList() {
    set_klass(ListKlass::get_instance());
    _inner_list = new ArrayList<HiObject*>();
}

HiList::HiList(ObjList ol) {
    set_klass(ListKlass::get_instance());
    _inner_list = ol;
}

ListAppendKlass* ListAppendKlass::instance = NULL;

ListAppendKlass* ListAppendKlass::get_instance() {
    if (instance == NULL)
        instance = new ListAppendKlass();

    return instance;
}

HiObject* ListAppendKlass::call(ObjList args) {
    ((HiList*)(args->get(0)))->inner_list()->add(args->get(1));
    return Universe::HiNone;
}

#include "object/hiList.hpp"
#include "object/hiTypeObject.hpp"
#include <assert.h>

ListKlass* ListKlass::instance = NULL;

ListKlass* ListKlass::get_instance() {
	if (instance == NULL)
		instance = new ListKlass();

	return instance;
}

ListKlass::ListKlass() {
    static NativeFunction* methods[] = {
        new ListAppendMethod(),
    };

    HiTypeObject* list_type_obj = new HiTypeObject();
    list_type_obj->set_methods(methods, 1);

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

HiObject* ListKlass::getattr(HiObject* obj, HiString* name) {
    return type_object()->get_type_attr(obj, name);
}


HiList::HiList() {
    set_klass(ListKlass::get_instance());
    _inner_list = new ArrayList<HiObject*>();
}

HiObject* ListAppendMethod::operator () (HiObject* x, ArgsList args) {
    HiList* lx = (HiList*)x;

    assert(args->length() == 1);
    assert(lx && lx->klass() == (Klass *)ListKlass::get_instance());
    lx->_inner_list->add(args->get(0));

    return NULL;
}

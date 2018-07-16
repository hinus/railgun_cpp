#include "object/hiDict.hpp"
#include "object/hiString.hpp"
#include <assert.h>

DictKlass* DictKlass::instance = NULL;

DictKlass* DictKlass::get_instance() {
	if (instance == NULL)
		instance = new DictKlass();

	return instance;
}

DictKlass::DictKlass() {
    set_klass_dict(new Map<HiObject*, HiObject*>());
    klass_dict()->put(new HiString("test"), new HiString("hwllo"));

    HiTypeObject* dict_type_obj = new HiTypeObject();
    set_type_object(dict_type_obj);
}

void DictKlass::print(HiObject* obj) {
    HiDict* dict_obj = (HiDict*) obj;
    assert(dict_obj && dict_obj->klass() == (Klass*) this);

    printf("{");   
    int size = dict_obj->_map->size();
    if (size >= 1) {
        dict_obj->_map->entries()[0]._k->print();
        printf(":");   
        dict_obj->_map->entries()[0]._v->print();
    }

    for (int i = 1; i < size; i++) {
        printf(", ");   
        dict_obj->_map->entries()[i]._k->print();
        printf(":");   
        dict_obj->_map->entries()[i]._v->print();
    }

    printf("}");
}

HiObject* DictKlass::getattr(HiObject* obj, HiString* name) {
    assert(obj->klass() == (Klass*) this);
    return klass_dict()->get((HiObject*)name);
}

HiObject* DictKlass::subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == (Klass*) this);
    return ((HiDict*)x)->map()->get(y);
}

HiDict::HiDict() {
    _map = new Map<HiObject*, HiObject*>();
    set_klass(DictKlass::get_instance());
}

HiDict::HiDict(NameTable x) {
    _map = x;
    set_klass(DictKlass::get_instance());
}


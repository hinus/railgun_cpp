#include "object/hiDict.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "runtime/universe.hpp"
#include "memory/oopClosure.hpp"
#include <assert.h>

DictKlass* DictKlass::instance = NULL;

DictKlass* DictKlass::get_instance() {
    if (instance == NULL) {
        instance = new DictKlass();
    }

    return instance;
}

DictKlass::DictKlass() {
    set_super(Universe::object_klass);

    HiTypeObject* dict_type_obj = new HiTypeObject();
    set_type_object(dict_type_obj);
}

void DictKlass::initialize() {
    HiDict* klass_dict = new HiDict();
    klass_dict->put(new HiString("test"), new HiString("hello"));
    set_klass_dict(klass_dict);
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

size_t DictKlass::size() {
    return sizeof(HiDict);
}

void DictKlass::oops_do(OopClosure* f, HiObject* obj) {
    assert(obj->klass() == (Klass*)this);

    f->do_map(&((HiDict*)obj)->_map);
}

HiObject* DictKlass::getattr(HiObject* obj, HiString* name) {
    assert(obj->klass() == (Klass*) this);
    return klass_dict()->get((HiObject*)name);
}

HiObject* DictKlass::subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == (Klass*) this);
    return ((HiDict*)x)->map()->get(y);
}

void DictKlass::store_subscr(HiObject* x, HiObject*y, HiObject* z) {
    assert(x && x->klass() == (Klass*) this);
    ((HiDict*)x)->put(y, z);
}

HiObject* DictKlass::iter(HiObject* x) {
    assert(x && x->klass() == (Klass*) this);
    HiObject* obj = new HiObject();
    obj->set_klass(DictIteratorKlass::get_instance());

    obj->setattr(new HiString("dobj"), x);
    obj->setattr(new HiString("iter_cnt"), new HiInteger(0));

    return obj;
}

HiDict::HiDict() {
    _map = new Map<HiObject*, HiObject*>();
    set_klass(DictKlass::get_instance());
}

HiDict::HiDict(NameTable x) {
    _map = x;
    set_klass(DictKlass::get_instance());
}

/*
 * Iterations for dict object
 */
DictIteratorKlass* DictIteratorKlass::instance = NULL;

DictIteratorKlass* DictIteratorKlass::get_instance() {
    if (instance == NULL) {
        instance = new DictIteratorKlass();
    }

    return instance;
}

DictIteratorKlass::DictIteratorKlass() {
}

HiObject* DictIteratorKlass::next(HiObject* x) {
    HiString* attr_name = new HiString("iter_cnt");
    HiInteger* iter_cnt = (HiInteger*)(x->getattr(attr_name));

    HiDict* adict = (HiDict*)(x->getattr(new HiString("dobj")));
    if (iter_cnt->value() < adict->map()->size()) {
        HiObject* obj = adict->map()->get_key(iter_cnt->value());
        HiObject* cnt = iter_cnt->add(new HiInteger(1));
        x->setattr(attr_name, cnt);
        return obj;
    }
    else // TODO : we need Traceback here to mark iteration end
        return NULL;
}


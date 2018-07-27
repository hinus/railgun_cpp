#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "object/hiDict.hpp"
#include "object/hiString.hpp"
#include "runtime/universe.hpp"
#include "memory/oopClosure.hpp"
#include "runtime/functionObject.hpp"
#include <assert.h>

ListKlass* ListKlass::instance = NULL;

ListKlass* ListKlass::get_instance() {
    if (instance == NULL)
        instance = new ListKlass();

    return instance;
}

ListKlass::ListKlass() {
    HiDict * klass_dict = new HiDict();
    klass_dict->put(new HiString("append"), 
        new FunctionObject(ListAppendKlass::get_instance()));

    HiTypeObject* list_type_obj = new HiTypeObject();

    set_klass_dict(klass_dict);
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

size_t ListKlass::size() {
    return sizeof(HiList);
}

HiObject* ListKlass::iter(HiObject* x) {
    assert(x && x->klass() == (Klass*) this);

    HiObject* iterator = new HiObject();
    iterator->set_klass(ListIteratorKlass::get_instance());
    iterator->setattr(new HiString("list"), x);
    iterator->setattr(new HiString("iter_cnt"), new HiInteger(0));

    return iterator;
}

void ListKlass::oops_do(OopClosure* f, HiObject* obj) {
    assert(obj && obj->klass() == (Klass*) this);

    f->do_array_list(&((HiList*)obj)->_inner_list);
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

ListAppendKlass::ListAppendKlass() {
    set_super(NativeFunctionKlass::get_instance());
}

HiObject* ListAppendKlass::call(ObjList args) {
    ((HiList*)(args->get(0)))->append(args->get(1));
    return Universe::HiNone;
}

/*
 * List Iterators
 */
ListIteratorKlass* ListIteratorKlass::instance = NULL;

ListIteratorKlass* ListIteratorKlass::get_instance() {
    if (instance == NULL)
        instance = new ListIteratorKlass();

    return instance;
}

HiObject* ListIteratorKlass::next(HiObject* x) {
    HiString* attr_name = new HiString("iter_cnt");
    HiObject* iter_cnt = x->getattr(attr_name);

    HiList* alist = (HiList*)(x->getattr(new HiString("list")));
    if (((HiInteger*)iter_cnt)->value() < alist->inner_list()->size()) {
        HiObject* obj = alist->subscr(iter_cnt);
        iter_cnt = iter_cnt->add(new HiInteger(1));
        x->setattr(attr_name, iter_cnt);
        return obj;
    }
    else // TODO : we need Traceback here to mark iteration end
        return NULL;
}


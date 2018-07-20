#include "memory/oopClosure.hpp"
#include "memory/heap.hpp"
#include "object/hiObject.hpp"

ScavengeOopClosure::ScavengeOopClosure(Space* from, Space* to) {
    _from = from;
    _to = to;
}

void ScavengeOopClosure::do_oop(HiObject** oop) {
    if (oop == NULL || *oop == NULL)
        return;

    HiObject* obj = (*oop);
    char* target = obj->new_address();
    if (target) {
        (*(char**)oop) = target;
        return;
    }

    size_t size = obj->size();
    target = (char*)_to->allocate(size);
    memcpy(target, obj, size);
    (*(char**)oop) = target;
    obj->set_new_address(target);

    obj = (HiObject*) target;
    obj->oops_do(this);
}

void ScavengeOopClosure::do_array_list(ArrayList<Klass*>** alist) {
    if (alist == NULL || *alist == NULL)
        return;

    size_t size = sizeof(ArrayList<Klass*>);
    char* target = (char*)_to->allocate(size);
    memcpy(target, (*alist), size);
    (*(char**)alist) = target;

    // since we do not move Klass, we can inline it here
    for (int i = 0; i < (*alist)->size(); i++) {
        do_klass((*alist)->get(i));
    }
}

void ScavengeOopClosure::do_array_list(ArrayList<HiObject*>** alist) {
    if (*alist == NULL)
        return;

    size_t size = sizeof(ArrayList<HiObject*>);
    char* target = (char*)_to->allocate(size);
    memcpy(target, (*alist), size);
    (*(char**)alist) = target;
    (*alist)->oops_do(this);
}

void ScavengeOopClosure::do_map(Map<HiObject*, HiObject*>** amap) {
    if (*amap == NULL)
        return;

    size_t size = sizeof(Map<HiObject*, HiObject*>);
    char* target = (char*)_to->allocate(size);
    memcpy(target, (*amap), size);
    (*(char**)amap) = target;
    (*amap)->oops_do(this);
}

void ScavengeOopClosure::do_raw_mem(char** mem, int length) {
    if (*mem == NULL)
        return;

    char* target = (char*)_to->allocate(length);
    memcpy(target, (*mem), length);
    (*mem) = target;
}

// we do not move klass
void ScavengeOopClosure::do_klass(Klass* k) {
    do_map(k->klass_dict_address());
}


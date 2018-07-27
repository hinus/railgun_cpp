#ifndef HI_DICT_HPP
#define HI_DICT_HPP

#include "object/hiObject.hpp"

class DictKlass : public Klass {
private:
    DictKlass();
    static DictKlass* instance;

public:
    static DictKlass* get_instance();
    void initialize();

    virtual HiObject* getattr(HiObject* x, HiString* y);
    virtual HiObject* subscr (HiObject* x, HiObject* y);

    virtual HiObject* iter(HiObject* x);
    virtual void print(HiObject* obj);
    virtual void store_subscr(HiObject* x, HiObject* y, HiObject* z);

    virtual size_t size();
    virtual void oops_do(OopClosure* f, HiObject* obj);
};

class DictIteratorKlass : public Klass {
private:
    static DictIteratorKlass* instance;
    DictIteratorKlass();

public:
    static DictIteratorKlass* get_instance();

    virtual HiObject* next(HiObject* x);
};

class HiDict : public HiObject {
friend class DictKlass;
private:
    NameTable _map;

public:
    HiDict();
    HiDict(NameTable map);
    Map<HiObject*, HiObject*>* map()   { return _map; }
    void put(HiObject* k, HiObject* v) { _map->put(k, v); }
    HiObject* get(HiObject* k)         { return _map->get(k); }
    HiObject* remove(HiObject* k)      { return _map->remove(k); }
};

#endif

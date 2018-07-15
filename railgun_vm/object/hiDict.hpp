#ifndef HI_DICT_HPP
#define HI_DICT_HPP

#include "object/hiObject.hpp"

class DictKlass : public Klass {
private:
	DictKlass();
	static DictKlass* instance;

public:
	static DictKlass* get_instance();

    virtual HiObject* getattr(HiObject* x, HiString* y);
	virtual void print(HiObject* obj);
};


class HiDict : public HiObject {
friend class DictKlass;
private:
    Map<HiObject*, HiObject*>* _map;

public:
    HiDict();
    Map<HiObject*, HiObject*>* map() { return _map; }
    void put(HiObject* k, HiObject* v) { _map->put(k, v); }
};

#endif

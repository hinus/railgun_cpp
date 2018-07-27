#include "runtime/stringTable.hpp"
#include "memory/oopClosure.hpp"
#include "object/hiString.hpp"

StringTable* StringTable::instance = NULL;

StringTable* StringTable::get_instance() {
    if (instance == NULL)
        instance = new StringTable();

    return instance;
}

StringTable::StringTable() {
    init_str = new HiString("__init__");
    add_str = new HiString("__add__");
    sub_str = new HiString("__sub__");
    mul_str = new HiString("__mul__");
    div_str = new HiString("__div__");

    and_str = new HiString("__and__");
    or_str = new HiString("__or__");
    xor_str = new HiString("__xor__");

    repr_str = new HiString("__repr__");
    call_str = new HiString("__call__");
    next_str = new HiString("next");
    iter_str = new HiString("__iter__");

    getitem_str = new HiString("__getitem__");
    setitem_str = new HiString("__setitem__");
}

void StringTable::oops_do(OopClosure* f) {
    f->do_oop((HiObject**)&init_str);
    f->do_oop((HiObject**)&add_str);
    f->do_oop((HiObject**)&sub_str);
    f->do_oop((HiObject**)&mul_str);
    f->do_oop((HiObject**)&div_str);

    f->do_oop((HiObject**)&and_str);
    f->do_oop((HiObject**)&or_str);
    f->do_oop((HiObject**)&xor_str);

    f->do_oop((HiObject**)&repr_str);
    f->do_oop((HiObject**)&call_str);
    f->do_oop((HiObject**)&next_str);
    f->do_oop((HiObject**)&iter_str);

    f->do_oop((HiObject**)&getitem_str);
    f->do_oop((HiObject**)&setitem_str);
}


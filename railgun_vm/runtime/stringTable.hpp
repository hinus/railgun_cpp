#ifndef STRING_TABLE_HPP
#define STRING_TABLE_HPP

class HiString;
class OopClosure;

class StringTable {
private:
    static StringTable* instance;
    StringTable();

public:
    static StringTable* get_instance();
    
    HiString* init_str;
    HiString* add_str;
    HiString* sub_str;
    HiString* mul_str;
    HiString* div_str;

    HiString* and_str;
    HiString* or_str;
    HiString* xor_str;

    HiString* repr_str;
    HiString* call_str;
    HiString* next_str;
    HiString* iter_str;

    HiString* getitem_str;
    HiString* setitem_str;

    void oops_do(OopClosure* f);
};

#endif

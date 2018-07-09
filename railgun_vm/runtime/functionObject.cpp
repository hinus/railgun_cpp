#include "runtime/functionObject.hpp"

FunctionObject::FunctionObject(HiObject* code_object, 
    ArrayList<HiObject*>* globals) {
    CodeObject* co = (CodeObject*) code_object;

    _func_code = co;
    _func_name = co->_co_name;
    _consts    = co->_consts;
    _globals   = globals;

    _func_name->print();
    printf("\n");
}

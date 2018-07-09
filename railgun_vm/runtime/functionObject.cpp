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

void FunctionObject::set_default(ArrayList<HiObject*>* defaults) {
	if (defaults == NULL) {
		_defaults = NULL;
		return;
	}

	_defaults = new ArrayList<HiObject*>(defaults->length());

	for (int i = 0; i < defaults->length(); i++) {
		_defaults->set(i, defaults->get(i));
	}
}

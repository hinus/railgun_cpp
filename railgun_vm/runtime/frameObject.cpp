#include "runtime/frameObject.hpp"

FrameObject::FrameObject(CodeObject* codes, ArrayList<HiObject*>* globals,
		ArrayList<HiObject*>* locals) {
    _consts = codes->_consts;
    _globals = globals;

	_locals = new ArrayList<HiObject*>(codes->_nlocals);
	if (locals) {
		for (int i = 0; i < locals->length(); i++) {
    		_locals->set(i, locals->get(i));
		}
	}

    _stack  = new Stack<HiObject*>(codes->_stack_size);
    _loop_stack  = new Stack<int>();

    _codes = codes;
    _pc    = 0;

    _ret_value = NULL;
    _sender    = NULL;
}

FrameObject::FrameObject (FunctionObject* func, ArrayList<HiObject*>* args) {
    _codes  = func->_func_code;
    _globals = func->_globals;
    _consts = func->_consts;

	_locals = new ArrayList<HiObject*>(_codes->_nlocals);

	if (func->_defaults) {
		int dft_cnt = func->_defaults->length();
		int argcnt  = _codes->_argcount;

		while (dft_cnt--) {
			_locals->set(--argcnt, func->_defaults->get(dft_cnt));
		}
	}

	if (args) {
		for (int i = 0; i < args->length(); i++) {
    		_locals->set(i, args->get(i));
		}
	}

    _stack  = new Stack<HiObject*>(_codes->_stack_size);
    _loop_stack  = new Stack<int>();

    _pc    = 0;

    _ret_value = NULL;
    _sender    = NULL;
}


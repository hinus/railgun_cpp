#include "runtime/frameObject.hpp"

// this constructor is used for module only.
FrameObject::FrameObject(CodeObject* codes, NameTable globals) {
    _consts  = codes->_consts;
    _names   = codes->_names;
    
    _globals = globals;
    _locals  = globals;
    _fast_locals = new ArrayList<HiObject*>(codes->_nlocals);

    _stack  = new Stack<HiObject*>(codes->_stack_size);
    _loop_stack  = new Stack<int>();

    _codes = codes;
    _pc    = 0;
    _sender    = NULL;
}

FrameObject::FrameObject (FunctionObject* func, ArrayList<HiObject*>* args) {
    _codes   = func->_func_code;
    _globals = func->_globals;
    _consts  = _codes->_consts;
    _names   = _codes->_names;

    _locals = new Map<HiObject*, HiObject*>();
    _fast_locals = new ArrayList<HiObject*>(_codes->_nlocals);

    if (func->_defaults) {
        int dft_cnt = func->_defaults->length();
        int argcnt  = _codes->_argcount;

        while (dft_cnt--) {
            _fast_locals->set(--argcnt, func->_defaults->get(dft_cnt));
        }
    }

    if (args) {
        for (int i = 0; i < args->length(); i++) {
            _fast_locals->set(i, args->get(i));
        }
    }

    _stack  = new Stack<HiObject*>(_codes->_stack_size);
    _loop_stack  = new Stack<int>();

    _pc    = 0;
    _sender    = NULL;
}

FrameObject::~FrameObject() {
    delete _loop_stack;
    delete _stack;
}

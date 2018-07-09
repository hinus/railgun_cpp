#include "runtime/frameObject.hpp"

FrameObject::FrameObject(CodeObject* codes, ArrayList<HiObject*>* globals) {
    _consts = codes->_consts;
    _globals = globals;

    _locals = new ArrayList<HiObject*>(codes->_nlocals);
    _stack  = new Stack<HiObject*>(codes->_stack_size);
    _loop_stack  = new Stack<int>();

    _codes = codes;
    _pc    = 0;

    _ret_value = NULL;
    _sender    = NULL;
}

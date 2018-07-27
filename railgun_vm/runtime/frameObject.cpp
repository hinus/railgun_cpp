#include "runtime/frameObject.hpp"
#include "memory/oopClosure.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "runtime/generator.hpp"

// this constructor is used for module only.
FrameObject::FrameObject(CodeObject* codes, HiDict* globals) {
    _consts  = codes->_consts;
    _names   = codes->_names;
    
    _globals = globals;
    _locals  = globals;
    _fast_locals = new HiList();

    _stack  = new HiList();
    _loop_stack  = new Stack<Block>();

    _codes = codes;
    _pc    = 0;
    _sender    = NULL;

    _entry_frame = false;
}

FrameObject::FrameObject (FunctionObject* func, ArrayList<HiObject*>* args) {
    _codes   = func->_func_code;
    _globals = func->_globals;
    _consts  = _codes->_consts;
    _names   = _codes->_names;

    _locals = new HiDict();
    _fast_locals = new HiList();

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

    _stack  = new HiList();
    _loop_stack  = new Stack<Block>();

    _pc    = 0;
    _sender    = NULL;
    _entry_frame = false;
}

FrameObject::FrameObject(Generator* g) {
    _consts  = g->_consts;
    _names   = g->_names;
    
    _globals = g->_globals;
    _locals  = g->_locals;
    _fast_locals = g->_fast_locals;

    _stack  = g->_stack;
    _loop_stack  = new Stack<Block>();
    _loop_stack->copy(g->_loop_stack);

    _codes = g->_codes;
    _pc    = g->_pc;
    _sender    = NULL;
    _entry_frame = true;
}

FrameObject::~FrameObject() {
    delete _loop_stack;
    _stack = NULL;
}

int FrameObject::get_op_arg(bool x86) {
    if (x86) {
        return _codes->_bytecodes->value()[_pc++] & 0xff;
    }
    else {
        int byte1 = _codes->_bytecodes->value()[_pc++] & 0xff;
        int byte2 = _codes->_bytecodes->value()[_pc++] & 0xff;
        return byte2 << 8 | byte1;
    }
}

unsigned char FrameObject::get_op_code() {
    return _codes->_bytecodes->value()[_pc++];
}

bool FrameObject::has_more_codes() {
    return _pc < _codes->_bytecodes->length();
}

void FrameObject::oops_do(OopClosure* f) {
    f->do_array_list(&_consts);
    f->do_array_list(&_names);

    f->do_oop((HiObject**)&_globals);
    f->do_oop((HiObject**)&_locals);

    f->do_oop((HiObject**)&_fast_locals);
    f->do_oop((HiObject**)&_stack);

    f->do_oop((HiObject**)&_codes);

    if (_sender)
        _sender->oops_do(f);
}


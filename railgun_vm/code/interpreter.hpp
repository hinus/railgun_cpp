#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"
#include "util/stack.hpp"
#include "util/map.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/frameObject.hpp"

class Interpreter {
private:
    Stack<HiObject*>*     _stack;
    Stack<int>*           _loop_stack;
    ArrayList<HiObject*>* _consts;
    ArrayList<HiObject*>* _names;
    ArrayList<HiObject*>* _fast_locals;
    NameTable             _locals_table;
    NameTable             _globals_table;
    FrameObject*          _top_frame;
    CodeObject*           _codes;
    const char*           _bytecodes;
    int                   _code_length;
    int                   _pc;

public:
    Interpreter() {};

    void      run        (CodeObject* codes);
    void      call_func  (HiObject* func, ArrayList<HiObject*>* args);
    void      enter_frame(FrameObject* frame);
    void      eval_code  ();
    HiObject* leave_last_frame();
};

#endif

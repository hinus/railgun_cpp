#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"
#include "util/stack.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/frameObject.hpp"

class Interpreter {
private:
	Stack<HiObject*>*     _stack;
	Stack<int>*           _loop_stack;
	ArrayList<HiObject*>* _consts;
	ArrayList<HiObject*>* _locals_table;
	ArrayList<HiObject*>* _globals_table;
    FrameObject*          _top_frame;
    CodeObject*           _codes;
    int                   _pc;

public:
	Interpreter() {};

	void      run        (CodeObject* codes);
	HiObject* call       (FunctionObject* func);
    void      eval_frame (FrameObject* frame);
    void      eval_code  ();
    void      remove_last_frame(int &length, const char*& code_value);
};

#endif

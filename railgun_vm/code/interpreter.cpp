#include "code/interpreter.hpp"
#include "object/hiInteger.hpp"

#include <assert.h>

void Interpreter::run(CodeObject* codes) {
    FrameObject* frame = new FrameObject(codes, 
        new ArrayList<HiObject*>(codes->_nlocals));

    _top_frame = NULL;
    eval_frame(frame);
}

HiObject* Interpreter::call(FunctionObject* func) {
    CodeObject* codes = func->_func_code;
    FrameObject* frame = new FrameObject(codes, func->_globals);

    eval_frame(frame);
    return frame->_ret_value;
}

void Interpreter::eval_frame(FrameObject* frame) {
	_stack         = frame->_stack;
	_loop_stack    = frame->_loop_stack;
	_locals_table  = frame->_locals;
	_consts        = frame->_consts;
    _globals_table = frame->_globals;

    frame->set_sender(_top_frame);
    _top_frame     = frame;

    _codes          = frame->_codes;
    _pc            = 0;

    eval_code();
}

void Interpreter::remove_last_frame(int &code_length, const char *& bytecodes) {
    if (!_top_frame->sender()) {
        delete _top_frame;
        _top_frame = NULL;
        return;
    }

    FrameObject* temp = _top_frame;
    _top_frame     = _top_frame->sender();
    _codes         = _top_frame->_codes;
    _pc            = _top_frame->get_pc();

	_stack         = _top_frame->_stack;
	_loop_stack    = _top_frame->_loop_stack;
	_locals_table  = _top_frame->_locals;
	_consts        = _top_frame->_consts;
    _globals_table = _top_frame->_globals;

    code_length = _codes->_bytecodes->length();
    bytecodes   = _codes->_bytecodes->value();

    delete temp;
}

void Interpreter::eval_code() {
	_pc = 0;
	int code_length = _codes->_bytecodes->length();
	const char* bytecodes = _codes->_bytecodes->value();
	bool x86 = false;

	while (_pc < code_length) {
		unsigned char op_code = bytecodes[_pc++];
		bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

		int op_arg = -1;
		if (has_argument) {
            op_arg = x86 ? (bytecodes[_pc++] & 0xFF) : 
		        ((bytecodes[_pc++] & 0xFF) + ((bytecodes[_pc++] & 0xFF) << 8));
        }

        HiObject* v, * w, * u, * attr;

		switch (op_code) {
            case ByteCode::POP_TOP:
                _stack->pop();
                break;

            case ByteCode::ROT_TWO:
                v = _stack->pop();
                w = _stack->pop();
                _stack->push(v);
                _stack->push(w);
                break;

            case ByteCode::ROT_THREE:
                v = _stack->pop();
                w = _stack->pop();
                u = _stack->pop();
                _stack->push(v);
                _stack->push(u);
                _stack->push(w);
                break;

			case ByteCode::LOAD_CONST:
				_stack->push(_consts->get(op_arg));
                break;

            case ByteCode::LOAD_GLOBAL:
			case ByteCode::LOAD_NAME:
				_stack->push(_globals_table->get(op_arg));
                break;

			case ByteCode::PRINT_ITEM:
				v = _stack->pop();
				v->print();
				break;

			case ByteCode::PRINT_NEWLINE:
				printf("\n");
				break;

            case ByteCode::STORE_GLOBAL:
			case ByteCode::STORE_NAME:
                _globals_table->set(op_arg, _stack->pop());
				break;

			case ByteCode::COMPARE_OP:
                w = _stack->pop();
                v = _stack->pop();

                switch(op_arg) {
                case ByteCode::GREATER:
                    _stack->push(v->greater(w));
                    break;
                case ByteCode::LESS:
                    _stack->push(v->less(w));
                    break;
                case ByteCode::EQUAL:
                    _stack->push(v->equal(w));
                    break;
                case ByteCode::NOT_EQUAL:
                    _stack->push(v->not_equal(w));
                    break;
                case ByteCode::GREATER_EQUAL:
                    _stack->push(v->ge(w));
                    break;
                case ByteCode::LESS_EQUAL:
                    _stack->push(v->le(w));
                    break;
                }
                break;

            case ByteCode::SETUP_LOOP:
                _loop_stack->push(_pc + op_arg);
                break;

            case ByteCode::POP_BLOCK:
                _loop_stack->pop();
                break;

            case ByteCode::POP_JUMP_IF_FALSE:
                v = _stack->pop();
                assert(v->klass() == IntegerKlass::get_instance());
                if (((HiInteger*)v)->value() == 0) {
                    _pc = op_arg;
                }
                break;

            case ByteCode::JUMP_FORWARD:
                _pc += op_arg;
                break;
                
            case ByteCode::JUMP_ABSOLUTE:
                _pc = op_arg;
                break;
                
            case ByteCode::BREAK_LOOP:
                _pc = _loop_stack->pop();
                break;
                
            case ByteCode::RETURN_VALUE:
                _top_frame->_ret_value = _stack->pop(); 
                remove_last_frame(code_length, bytecodes);
                break;

            case ByteCode::INPLACE_ADD: // drop down
            case ByteCode::BINARY_ADD:
                v = _stack->pop();
                w = _stack->pop();
                _stack->push(w->add(v));
                break;

            case ByteCode::INPLACE_SUBSTRACT: // drop down
            case ByteCode::BINARY_SUBSTRACT:
                v = _stack->pop();
                w = _stack->pop();
                _stack->push(w->sub(v));
                break;

            case ByteCode::INPLACE_MULTIPLY: // drop down
            case ByteCode::BINARY_MULTIPLY:
                v = _stack->pop();
                w = _stack->pop();
                _stack->push(w->mul(v));
                break;

            case ByteCode::INPLACE_DIVIDE: // drop down
            case ByteCode::BINARY_DIVIDE:
                v = _stack->pop();
                w = _stack->pop();
                _stack->push(w->div(v));
                break;

            case ByteCode::INPLACE_MODULO: // drop down
            case ByteCode::BINARY_MODULO:
                v = _stack->pop();
                w = _stack->pop();
                _stack->push(w->mod(v));
                break;

            case ByteCode::MAKE_FUNCTION:
                v = _stack->pop();
                _stack->push(new FunctionObject(v, _globals_table));
                break;

            case ByteCode::CALL_FUNCTION:
                v = _stack->pop();
                _top_frame->set_pc(_pc);
                _stack->push(call((FunctionObject*)v));
                break;

			default:
				// do nothing
				break;
		}
	}
}

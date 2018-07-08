#include "code/interpreter.hpp"
#include "object/hiInteger.hpp"

#include <string.h>

Interpreter::Interpreter() {
}

void Interpreter::run(CodeObject* codes) {
	int pc = 0;
	int code_length = codes->_bytecodes->length();
	const char* bytecodes = codes->_bytecodes->value();
	bool x86 = false;

	_stack  = new Stack<HiObject*>(codes->_stack_size);
	_consts = codes->_consts;

	while (pc < code_length) {
		unsigned char op_code = bytecodes[pc++];
		bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

		int op_arg = -1;
		if (has_argument) {
                op_arg = x86 ? (bytecodes[pc++] & 0xFF) : 
					((bytecodes[pc++] & 0xFF) + ((bytecodes[pc++] & 0xFF) << 8));
        }

		HiInteger* lhs, * rhs;
        HiObject* v, * w, * u, * attr;

		switch (op_code) {
			case ByteCode::LOAD_CONST:
				_stack->push(_consts->get(op_arg));
                break;
			case ByteCode::PRINT_ITEM:
				v = _stack->pop();
				v->print();
				break;
			case ByteCode::PRINT_NEWLINE:
				printf("\n");
				break;
			default:
				// do nothing
				break;
		}
	}
}
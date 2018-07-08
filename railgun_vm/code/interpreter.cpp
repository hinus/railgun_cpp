#include "code/interpreter.hpp"
#include "object/hiInteger.hpp"

#include <string.h>

Interpreter::Interpreter() {
}

void Interpreter::run(CodeObject* codes) {
	int pc = 0;
	int code_length = strlen(codes->_bytecodes);
	bool x86 = false;

	_stack  = new Stack<HiObject*>(codes->_stack_size);
	_consts = codes->_consts;

	while (pc < code_length) {
		unsigned char op_code = codes->_bytecodes[pc++];
		bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

		int op_arg = -1;
		if (has_argument) {
                op_arg = x86 ? (codes->_bytecodes[pc++] & 0xFF) : 
					((codes->_bytecodes[pc++] & 0xFF) + ((codes->_bytecodes[pc++] & 0xFF) << 8));
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
		}
	}
}
#include <iostream>

#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "code/codeObject.hpp"
#include "code/interpreter.hpp"


int main(int argc, char** argv) {
    BufferedInputStream input_stream(argv[1]);
	BinaryFileParser parser(&input_stream);
	Interpreter interpreter;

	interpreter.run(parser.parse());

	return 0;
}
/*
    HiObject** consts;
    HiObject ** variables;

    Universe::genesis();

    int const_cnt = input_stream.read();
    if (const_cnt > 0)
        consts = new HiObject*[const_cnt];

    for (int i = 0; i < const_cnt; i++) {
        char type = input_stream.read();

        if (type == 0) {
            consts[i] = new HiInteger(input_stream.read_int());
        } else if (type == 1) {
            int length = input_stream.read_int();
            char* str = new char[length + 1];

            for (int j = 0; j < length; j++) {
                str[j] = input_stream.read();
            }

            str[length] = '\0';
            //std::cout << str << std::endl;
            consts[i] = new HiString(str);
        }
    }

    int var_cnt = input_stream.read();

    if (var_cnt > 0)
        variables = new HiObject*[var_cnt];
    
    // infos for debug, in fact, variables infos are not neccessary.
    for (int i = 0; i < var_cnt; i++) {
        char type = input_stream.read();

        if (type == 1) {
            int length = input_stream.read_int();
            char* str = new char[length + 1];

            for (int j = 0; j < length; j++) {
                str[j] = input_stream.read();
            }

            str[length] = '\0';

            //std::cout << str << std::endl;
        }
    }

    // read codes
    int code_length = input_stream.read_int();
    char* byte_codes = new char[code_length];

    for (int i = 0; i < code_length; i++) {
        byte_codes[i] = input_stream.read();
    }

    int pc = 0;

    Stack<HiObject *> stack(10);
    Stack<int> loop_stack(10);

    int operand = 0;
    Op cur_op;

    HiObject *x, *y;
    HiInteger *ix, *iy;

    while (pc < code_length) {
        cur_op = (Op)byte_codes[pc++];
#ifdef DEBUG
        printf("pc is %d, cur_op is %d\n", pc, cur_op);
#endif

        if (cur_op > OP_WITH_ARG)
            operand = byte_codes[pc++];

        switch (cur_op) {
        case LOAD_CONST:
            stack.push(consts[operand]);
            break;

        case LOAD_NAME:
            stack.push(variables[operand]);
            break;

        case STORE_NAME:
#ifdef DEBUG
            printf("STORE_NAME : %d\n", operand);
#endif
            variables[operand] = stack.pop();
            break;

        case JMP_IF_FALSE:
            ix = (HiInteger *)stack.top();
            if (ix->value() == 0) {
                pc += operand;
#ifdef DEBUG
                printf("JMP_IF_FALSE : %d\n", pc);
#endif
            }
            break;

        case JMP_FORWARD:
            pc += operand;
            break;

        case POP_TOP:
            stack.pop();
            break;

        case COMPARE_OP:
            iy = (HiInteger *)stack.pop();
            ix = (HiInteger *)stack.pop();

            if (operand == 2) {
                if ((*ix) == (*iy))
                    stack.push(new HiInteger(1));
                else
                    stack.push(new HiInteger(0));
            }
            else if (operand == 0) {
                if ((*ix) < (*iy))
                    stack.push(new HiInteger(1));
                else
                    stack.push(new HiInteger(0));
            }
            else
                std::cout << "Unsupported comparator" << std::endl;

            break;

        case PRINT_ITEM:
            x = stack.pop();
            x->klass()->print(x);
            break;

        case PRINT_NEWLINE:
            std::cout << std::endl;
            break;

        case ADD:
            x = stack.pop();
            y = stack.pop();

            stack.push(x->klass()->add(x, y));
            break;

        case MUL:
            x = stack.pop();
            y = stack.pop();

            stack.push(x->klass()->mul(x, y));
            break;

        case SUB:
            x = stack.pop();
            y = stack.pop();

            stack.push(y->klass()->sub(y, x));
            break;

        case DIV:
            x = stack.pop();
            y = stack.pop();

            stack.push(y->klass()->div(y, x));
            break;

        case SETUP_LOOP:
            loop_stack.push(pc + operand);
            break;
            
        case POP_BLOCK:
            loop_stack.pop();
            break;

        case BREAK_LOOP:
            pc = loop_stack.pop();
            break;

        case JMP_ABSOLUTE:
            pc = operand;
            break;
        }
    }

    input_stream.close();

    if (0 != stack.len())
        printf("imbalance stack : %d\n", stack.len());
}
*/

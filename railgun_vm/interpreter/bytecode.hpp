#ifndef _OP_HPP
#define _OP_HPP

enum Op {
    PRINT_ITEM = 1,
    PRINT_NEWLINE,
    ADD,
    SUB,
    MUL,
    DIV,
    POP_TOP,
    POP_BLOCK,
    BREAK_LOOP,

    OP_WITH_ARG = 90,
    LOAD_CONST,
    LOAD_NAME,
    STORE_NAME,
    COMPARE_OP,
    JMP_IF_FALSE,
    JMP_FORWARD,
    JMP_ABSOLUTE,
    SETUP_LOOP,
};

#endif

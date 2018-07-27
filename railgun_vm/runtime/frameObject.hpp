#ifndef FRAME_OBJECT_HPP
#define FRAME_OBJECT_HPP

#include "code/codeObject.hpp"
#include "util/stack.hpp"
#include "util/map.hpp"
#include "util/arrayList.hpp"
#include "runtime/functionObject.hpp"

class OopClosure;
class Generator;
class HiList;

class Block {
public:
    unsigned char _type;
    unsigned int  _target;
    int  _level;

    Block() {
        _type = 0;
        _target = 0;
        _level = 0;
    }

    Block(unsigned char b_type,
            unsigned int b_target,
            int b_level):
        _type(b_type),
        _target(b_target),
        _level(b_level) {
        }

    Block(const Block& b) {
        _type = b._type;
        _target = b._target;
        _level  = b._level;
    }
};

class FrameObject {
friend class Generator;

public:
    FrameObject(CodeObject* codes, HiDict* globals);
    FrameObject(FunctionObject* func, ArrayList<HiObject*>* args);
    FrameObject(Generator* g);
    ~FrameObject();

    ArrayList<HiObject*>* _consts;
    ArrayList<HiObject*>* _names;

    HiDict*               _globals;
    HiDict*               _locals;
    HiList*               _fast_locals;

    HiList*               _stack;
    Stack<Block>*         _loop_stack;
    
    CodeObject*           _codes;
    int                   _pc;
    bool                  _entry_frame;
    FrameObject*          _sender;

public:
    void set_sender(FrameObject* x) { _sender = x; }
    FrameObject* sender()           { return _sender;}

    void set_pc(int x)              { _pc = x; }
    int  get_pc()                   { return _pc; }
    void set_entry_frame(bool x)    { _entry_frame = x; }
    bool is_entry_frame()           { return _entry_frame; }
    bool is_first_frame()           { return _sender == NULL; }

    HiList* stack()                 { return _stack; }

    bool has_more_codes();
    unsigned char get_op_code();
    int  get_op_arg(bool x86);

    void oops_do(OopClosure* f);
};

#endif

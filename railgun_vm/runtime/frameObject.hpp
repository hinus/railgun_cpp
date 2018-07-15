#ifndef FRAME_OBJECT_HPP
#define FRAME_OBJECT_HPP

#include "object/hiObject.hpp"
#include "code/codeObject.hpp"
#include "util/stack.hpp"
#include "util/map.hpp"
#include "util/arrayList.hpp"
#include "runtime/functionObject.hpp"

class FrameObject : public HiObject {
public:
    FrameObject(CodeObject* codes, NameTable globals);

    FrameObject(FunctionObject* func, ArrayList<HiObject*>* args);
    ~FrameObject();

    ArrayList<HiObject*>* _consts;
    ArrayList<HiObject*>* _names;

    NameTable             _globals;
    NameTable             _locals;
    ArrayList<HiObject*>* _fast_locals;

    Stack<HiObject*>*     _stack;
    Stack<int>*           _loop_stack;
    
    CodeObject*           _codes;
    int                   _pc;
    HiObject*             _ret_value;
    FrameObject*          _sender;

public:
    void set_sender(FrameObject* x) { _sender = x; }
    FrameObject* sender()           { return _sender;}

    void set_pc(int x)              { _pc = x; }
    int  get_pc()                   { return _pc; }
};

#endif

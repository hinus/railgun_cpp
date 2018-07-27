#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "object/hiObject.hpp"
#include "klass/klass.hpp"
#include "util/arrayList.hpp"
#include "util/stack.hpp"

class OopClosure;
class FunctionObject;
class CodeObject;
class HiDict;
class HiList;

class FrameObject;
class Interpreter;

class Block;

class GeneratorKlass : public Klass {
private:
    static GeneratorKlass* instance;
    GeneratorKlass();

public:
    static GeneratorKlass* get_instance();

    virtual HiObject* next(HiObject* obj);
    virtual HiObject* iter(HiObject* obj);

    virtual size_t size();
    virtual void oops_do(OopClosure* f, HiObject* obj);
};

class Generator : public HiObject {
friend class Interpreter;
friend class FrameObject;
friend class GeneratorKlass;

private:
    ArrayList<HiObject*>* _consts;
    ArrayList<HiObject*>* _names;

    HiDict*               _globals;
    HiDict*               _locals;
    HiList*               _fast_locals;

    HiList*               _stack;
    Stack<Block>*         _loop_stack;
    
    CodeObject*           _codes;
    int                   _pc;

public:
    Generator(FunctionObject* func, ArrayList<HiObject*>* args);
};

#endif

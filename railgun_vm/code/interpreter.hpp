#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/codeObject.hpp"
#include "util/map.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/frameObject.hpp"

class OopClosure;
class Generator;

class Interpreter {
    enum Status {
        IS_OK,
        IS_BREAK,
        IS_EXCEPTION,
        IS_RERAISE,
        IS_YIELD,
    };

private:
    static Interpreter*   _instance;
    Interpreter();

    NameTable             _builtins;
    HiObject*             _reraise_exception;
    HiObject*             _pending_exception;
    FrameObject*          _frame;

    Status                _int_status;

public:
    static Interpreter* get_instance();

    void      run             (CodeObject* codes);
    void      build_frame     (HiObject* func, ObjList args);
    void      enter_frame     (FrameObject* frame);
    void      save_frame      (Generator* g);
    void      eval_code       ();
    HiObject* eval_generator  (Generator* g);
    void      leave_last_frame();
    HiObject* call_virtual    (HiObject* func, ObjList args);


    void      oops_do         (OopClosure* f);
};

#endif

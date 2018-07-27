#include "klass/klass.hpp"
#include "code/bytecode.hpp"
#include "code/interpreter.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "object/hiString.hpp"
#include "runtime/universe.hpp"
#include "runtime/generator.hpp"
#include "memory/oopClosure.hpp"
#include "util/handles.hpp"

#include <assert.h>

#define POP()         _frame->stack()->pop()
#define TOP()         _frame->stack()->top()
#define PUSH(x)       _frame->stack()->append((x))
#define STACK_LEVEL() _frame->stack()->size()
#define PEEK(x)       _frame->stack()->get((x))

Interpreter* Interpreter::_instance = NULL;

Interpreter* Interpreter::get_instance() {
    if (_instance == NULL) {
        _instance = new Interpreter();
    }

    return _instance;
}

Interpreter::Interpreter() {
    _builtins = new Map<HiObject*, HiObject*>();

    _builtins->put(new HiString("__name__"), new HiString("__main__"));
    _builtins->put(new HiString("True"),     Universe::HiTrue);
    _builtins->put(new HiString("False"),    Universe::HiFalse);
    _builtins->put(new HiString("int"),      Universe::int_klass->type_object());
    _builtins->put(new HiString("object"),   Universe::object_klass->type_object());
    _builtins->put(new HiString("sysgc"),    new FunctionObject(SysGCKlass::get_instance()));

    _builtins->put(new HiString("StopIteration"),    Universe::stop_iteration);

    _frame = NULL;
    _pending_exception = NULL;

    _int_status = IS_OK;
}

void Interpreter::run(CodeObject* codes) {
    FrameObject* frame = new FrameObject(codes, new HiDict());

    enter_frame(frame);
    eval_code();
}

void Interpreter::build_frame(HiObject* callable, ObjList args) {
    if (MethodObject::is_native(callable)) {
        HiObject* result = ((FunctionObject*)callable)->call(args);
        // we do not create a virtual frame, but native frame.
        PUSH(result);
        return;
    }
    else if (MethodObject::is_method(callable)) {
        MethodObject* method = (MethodObject*) callable;
        // return value is ignored here, because they are handled
        // by other pathes.
        if (!args) {
            args = new ArrayList<HiObject*>(1);
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args);
    }
    else if (MethodObject::is_yield_function(callable)) {
        Generator* gtor = new Generator((FunctionObject*) callable, args);
        PUSH(gtor);
        return;
    }
    else if (MethodObject::is_function(callable)) {
        FrameObject* frame = new FrameObject((FunctionObject*) callable, args);
        enter_frame(frame);
    }
    else if (callable->klass() == Universe::type_klass) {
        HiObject* inst = new HiObject();
        inst->set_klass(((HiTypeObject*)callable)->own_klass());
        HiObject* constructor = inst->getattr(new HiString("__init__"));
        if (constructor != Universe::HiNone) {
            call_virtual(constructor, args);
        }
        PUSH(inst);
    }
    else {
        HiObject* m = callable->getattr(new HiString("__call__"));
        if (m != Universe::HiNone)
            build_frame(m, args);
        else {
            callable->print();
            printf("\nError : can not call a normal object.\n");
        }
    }
}

HiObject* Interpreter::call_virtual(HiObject* func, ObjList args) {
    if (MethodObject::is_native(func)) {
        // we do not create a virtual frame, but native frame.
        return ((FunctionObject*)func)->call(args);
    }
    else if (MethodObject::is_method(func)) {
        MethodObject* method = (MethodObject*) func;
        // return value is ignored here, because they are handled
        // by other pathes.
        if (!args) {
            args = new ArrayList<HiObject*>(1);
        }
        args->insert(0, method->owner());
        return call_virtual(method->func(), args);
    }
    else if (MethodObject::is_function(func)) {
        FrameObject* frame = new FrameObject((FunctionObject*) func, args);
        frame->set_entry_frame(true);
        enter_frame(frame);
        eval_code();
        return POP();
    }

    return Universe::HiNone;
}

void Interpreter::enter_frame(FrameObject* frame) {
    frame->set_sender(_frame);
    _frame         = frame;
}

void Interpreter::leave_last_frame() {
    if (!_frame->sender()) {
        delete _frame;
        _frame = NULL;
        return;
    }

    FrameObject* temp = _frame;
    _frame         = _frame->sender();

    delete temp;
}

void Interpreter::eval_code() {
    bool x86 = false;
    Block b;
    HiObject* v, * w, * u;
    FunctionObject* fo;
    ArrayList<HiObject*>* args = NULL;

    while (_frame->has_more_codes()) {
        unsigned char op_code = _frame->get_op_code();
        bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

        int op_arg = -1;
        if (has_argument) {
            op_arg = _frame->get_op_arg(x86);
        }

        switch (op_code) {
            case ByteCode::POP_TOP:
                POP();
                break;

            case ByteCode::DUP_TOP:
                PUSH(TOP());
                break;

            case ByteCode::ROT_TWO:
                v = POP();
                w = POP();
                PUSH(v);
                PUSH(w);
                break;

            case ByteCode::ROT_THREE:
                v = POP();
                w = POP();
                u = POP();
                PUSH(v);
                PUSH(u);
                PUSH(w);
                break;

            case ByteCode::BUILD_TUPLE: // drop down, we need this
            case ByteCode::BUILD_LIST:
                v = new HiList();
                while (op_arg--) {
                    ((HiList*)v)->set(op_arg, POP());
                }
                PUSH(v);
                break;

            case ByteCode::BUILD_MAP:
                v = new HiDict();
                PUSH(v);
                break;

            case ByteCode::BUILD_CLASS:
                v = POP();
                u = POP();
                w = POP();
                v = Klass::create_klass(v, u, w);
                PUSH(v);
                break;

            case ByteCode::LOAD_CONST:
                PUSH(_frame->_consts->get(op_arg));
                break;

            case ByteCode::LOAD_FAST:
                PUSH(_frame->_fast_locals->get(op_arg));
                break;

            case ByteCode::LOAD_GLOBAL:
                v = _frame->_names->get(op_arg);
                w = _frame->_globals->get(v);
                if (w == Universe::HiNone)
                    w = _builtins->get(v);

                PUSH(w);
                break;

            case ByteCode::LOAD_NAME:
                v = _frame->_names->get(op_arg);
                w = _frame->_locals->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _frame->_globals->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _builtins->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                PUSH(Universe::HiNone);
                break;

            case ByteCode::LOAD_ATTR:
                v = POP();
                w = _frame->_names->get(op_arg);
                PUSH(v->getattr(w));
                break;

            case ByteCode::LOAD_LOCALS:
                PUSH(_frame->_locals);
                break;

            case ByteCode::BINARY_SUBSCR:
                v = POP();
                w = POP();
                PUSH(w->subscr(v));
                break;

            case ByteCode::PRINT_ITEM:
                v = POP();
                v->print();
                break;

            case ByteCode::PRINT_NEWLINE:
                printf("\n");
                break;

            case ByteCode::STORE_FAST:
                _frame->_fast_locals->set(op_arg, POP());
                break;

            case ByteCode::STORE_GLOBAL:
                v = _frame->_names->get(op_arg);
                _frame->_globals->put(v, POP());
                break;

            case ByteCode::STORE_NAME:
                v = _frame->_names->get(op_arg);
                _frame->_locals->put(v, POP());
                break;

            case ByteCode::STORE_MAP:
                w = POP();
                u = POP();
                v = TOP();
                ((HiDict*)v)->put(w, u);
                break;

            case ByteCode::STORE_ATTR:
                u = POP();
                v = _frame->_names->get(op_arg);
                w = POP();
                u->setattr(v, w);
                break;

            case ByteCode::STORE_SUBSCR:
                u = POP();
                v = POP();
                w = POP();
                v->store_subscr(u, w);
                break;

            case ByteCode::DELETE_NAME:
                v = _frame->_names->get(op_arg);
                _frame->_locals->remove(v);
                break;

            case ByteCode::COMPARE_OP:
                w = POP();
                v = POP();

                switch(op_arg) {
                case ByteCode::GREATER:
                    PUSH(v->greater(w));
                    break;
                case ByteCode::LESS:
                    PUSH(v->less(w));
                    break;
                case ByteCode::EQUAL:
                    PUSH(v->equal(w));
                    break;
                case ByteCode::NOT_EQUAL:
                    PUSH(v->not_equal(w));
                    break;
                case ByteCode::GREATER_EQUAL:
                    PUSH(v->ge(w));
                    break;
                case ByteCode::LESS_EQUAL:
                    PUSH(v->le(w));
                    break;
                case ByteCode::EXC_MATCH:
                    if (v == w)
                        PUSH(Universe::HiTrue);
                    else
                        PUSH(Universe::HiFalse);
                }
                break;

            case ByteCode::SETUP_LOOP:
            case ByteCode::SETUP_FINALLY:
            case ByteCode::SETUP_EXCEPT:
                _frame->_loop_stack->push(Block(
                    op_code, _frame->_pc + op_arg,
                    STACK_LEVEL()));
                break;

            case ByteCode::END_FINALLY:
                // TODO: restore exceptions
                v = POP();
                if (v != Universe::HiNone) {
                    _pending_exception = v;
                    _int_status = IS_RERAISE;
                }
                break;

            case ByteCode::POP_BLOCK:
                b = _frame->_loop_stack->pop();
                while (STACK_LEVEL() > b._level) {
                    POP();
                }
                break;

            case ByteCode::YIELD_VALUE:
                // we are assured that we're in the progress
                // of evalating generator.
                _int_status = IS_YIELD;
                return;

            case ByteCode::POP_JUMP_IF_FALSE:
                v = POP();
                assert(v->klass() == IntegerKlass::get_instance());
                if (v == Universe::HiFalse)
                    _frame->_pc = op_arg;
                break;

            case ByteCode::JUMP_FORWARD:
                _frame->_pc += op_arg;
                break;

            case ByteCode::JUMP_IF_FALSE_OR_POP:
                v = TOP();
                if (v == Universe::HiFalse)
                    _frame->_pc = op_arg;
                else
                    POP();
                break;

            case ByteCode::POP_JUMP_IF_TRUE:
                v = POP();
                assert(v->klass() == IntegerKlass::get_instance());
                if (v == Universe::HiTrue) {
                    _frame->_pc = op_arg;
                }
                break;

            case ByteCode::JUMP_IF_TRUE_OR_POP:
                v = TOP();
                if (v == Universe::HiTrue)
                    _frame->_pc = op_arg;
                else
                    POP();
                break;

            case ByteCode::JUMP_ABSOLUTE:
                _frame->_pc = op_arg;
                break;

            case ByteCode::BREAK_LOOP:
                _int_status = IS_BREAK;
                break;

            case ByteCode::RETURN_VALUE:
                v = POP();
                if (_frame->is_first_frame()) {
                    return;
                }
                else {
                    if (_frame->is_entry_frame()) {
                        // CAUTION : this will change _frame and _stack
                        // so _stack below is different from above.
                        leave_last_frame();
                        PUSH(v);
                        return;
                    }
                    else {
                        leave_last_frame();
                        PUSH(v);
                    }
                }
                break;

            case ByteCode::GET_ITER:
                v = POP();
                PUSH(v->iter());
                break;

            case ByteCode::INPLACE_ADD: // drop down
            case ByteCode::BINARY_ADD:
                v = POP();
                w = POP();
                PUSH(w->add(v));
                break;

            case ByteCode::INPLACE_SUBSTRACT: // drop down
            case ByteCode::BINARY_SUBSTRACT:
                v = POP();
                w = POP();
                PUSH(w->sub(v));
                break;

            case ByteCode::INPLACE_MULTIPLY: // drop down
            case ByteCode::BINARY_MULTIPLY:
                v = POP();
                w = POP();
                PUSH(w->mul(v));
                break;

            case ByteCode::INPLACE_DIVIDE: // drop down
            case ByteCode::BINARY_DIVIDE:
                v = POP();
                w = POP();
                PUSH(w->div(v));
                break;

            case ByteCode::INPLACE_MODULO: // drop down
            case ByteCode::BINARY_MODULO:
                v = POP();
                w = POP();
                PUSH(w->mod(v));
                break;

            case ByteCode::BINARY_AND:
                v = POP();
                w = POP();
                PUSH(w->bi_and(v));
                break;

            case ByteCode::MAKE_FUNCTION:
                v = POP();
                fo = new FunctionObject(v, _frame->_globals);
                if (op_arg > 0) {
                    args = new ArrayList<HiObject*>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }
                fo->set_default(args);

                if (args != NULL) {
                    args = NULL;
                }

                PUSH(fo);
                break;

            case ByteCode::RAISE_VARARGS:
                switch (op_arg) {
                case 3:
                    u = POP();
                case 2:
                    v = POP();
                case 1:
                    w = POP();
                    break;
                }
                _pending_exception = w;
                _int_status = IS_EXCEPTION;

                break;

            case ByteCode::CALL_FUNCTION:
                if (op_arg > 0) {
                    args = new ArrayList<HiObject*>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }

                build_frame(POP(), args);
                if (args != NULL) {
                    args = NULL;
                }
                break;

             case ByteCode::DELETE_ATTR:
                v = POP();
                w = _frame->_names->get(op_arg);
                v->delattr(w);
                break;

            case ByteCode::UNPACK_SEQUENCE:
                v = POP();

                while (op_arg--) {
                    PUSH(v->subscr(new HiInteger(op_arg)));
                }
                break;

            case ByteCode::FOR_ITER:
                v = TOP();
                w = v->next();
                if (w != NULL)
                    PUSH(w);
                else {
                    _frame->_pc += op_arg;
                    POP();
                    // we may encounter a StopIteration, ignore it.
                    _int_status = IS_OK;
                    _pending_exception = NULL;
                }
                break;

            case ByteCode::LIST_APPEND:
                v = PEEK(op_arg);
                w = POP();
                assert(v->klass() == ListKlass::get_instance());
                ((HiList*)v)->append(w);
                break;

            case ByteCode::MAP_ADD:
                w = POP();
                u = POP();
                v = PEEK(op_arg - 1);
                assert(v->klass() == DictKlass::get_instance());
                ((HiDict*)v)->put(w, u);
                break;


            default:
                printf("Error : unrecognized bytecode %d\n", op_code);
                break;
        } // end of switch

        while (_int_status != IS_OK && !_frame->_loop_stack->empty()) {
            if (_int_status == IS_RERAISE)
                _int_status = IS_EXCEPTION;

            b = _frame->_loop_stack->pop();
            while (STACK_LEVEL() > b._level) {
                POP();
            }

            if (_int_status == IS_BREAK && b._type == ByteCode::SETUP_LOOP) {
                _frame->_pc = b._target;;
                _int_status = IS_OK;
            }
            else if (_int_status == IS_EXCEPTION) {
                if (b._type == ByteCode::SETUP_EXCEPT
                        || b._type == ByteCode::SETUP_FINALLY) {
                    PUSH(Universe::HiNone);
                    PUSH(Universe::HiNone);
                    PUSH(_pending_exception);
                    _pending_exception = NULL;
                    _frame->_pc = b._target;;
                    _int_status = IS_OK;
                }
            }
        }

        // has pending exception and no handler found, unwind stack.
        if (_int_status == IS_EXCEPTION && _frame->_loop_stack->empty()) {
            if (_frame->is_first_frame())
                return;

            if (_frame->is_entry_frame()) {
                // CAUTION : this will change _frame and _stack
                // so _stack below is different from above.
                leave_last_frame();
                PUSH(NULL);
                return;
            }
            else {
                leave_last_frame();
                PUSH(NULL);
            }
        }
    }
}

void Interpreter::oops_do(OopClosure* f) {
    f->do_map(&_builtins);

    if (_frame)
        _frame->oops_do(f);
}

HiObject* Interpreter::eval_generator(Generator* g) {
    Handle handle(g);
    FrameObject* frame = new FrameObject((Generator*)handle.resolve());
    enter_frame(frame);
    eval_code();

    if (_int_status != IS_YIELD) {
        _int_status = IS_OK;
        return NULL;
    }

    _int_status = IS_OK;
    HiObject* ret = TOP();
    save_frame((Generator*)handle.resolve());
    leave_last_frame();
    return ret;
}

void Interpreter::save_frame(Generator* g) {
    g->_consts      = _frame->_consts;
    g->_names       = _frame->_names;

    g->_globals     = _frame->_globals;
    g->_locals      = _frame->_locals;
    g->_fast_locals = _frame->_fast_locals;

    g->_stack       = _frame->_stack;
    g->_loop_stack->copy(_frame->_loop_stack);

    g->_codes       = _frame->_codes;
    g->_pc          = _frame->_pc;
}

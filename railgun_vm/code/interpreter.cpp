#include "klass/klass.hpp"
#include "code/interpreter.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "runtime/universe.hpp"

#include <assert.h>

Interpreter::Interpreter() {
    _builtins = new Map<HiObject*, HiObject*>();

    _builtins->put(new HiString("__name__"), new HiString("__main__"));
    _builtins->put(new HiString("True"),  Universe::HiTrue);
    _builtins->put(new HiString("False"), Universe::HiFalse);
    _builtins->put(new HiString("int"),   Universe::int_klass->type_object());
}

void Interpreter::run(CodeObject* codes) {
    FrameObject* frame = new FrameObject(codes, 
        new Map<HiObject*, HiObject*>());

    _top_frame = NULL;
    enter_frame(frame);
    eval_code();
}

void Interpreter::call_func(HiObject* callable, ArrayList<HiObject*>* args, bool with_ret_value) {
    if (MethodObject::is_native(callable)) {
        HiObject* result = ((FunctionObject*)callable)->call(args);
        // we do not create a virtual frame, but native frame.
        _stack->push(result);
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
        call_func(method->func(), args, with_ret_value);
    }
    else if (MethodObject::is_function(callable)) {
        _top_frame->set_with_ret_value(with_ret_value);
        FrameObject* frame = new FrameObject((FunctionObject*) callable, args);
        enter_frame(frame);
    }
    else if (callable->klass() == Universe::type_klass) {
        HiObject* inst = new HiObject();
        inst->set_klass(((HiTypeObject*)callable)->own_klass());
        _stack->push(inst);
        HiObject* constructor = inst->getattr(new HiString("__init__"));
        if (constructor) {
            call_func(constructor, args, false);
        }
    }
    else {
        HiObject* m = callable->getattr(new HiString("__call__"));
        if (m)
            call_func(m, args, with_ret_value);
        else
            printf("Error : can not call a normal object.\n");
    }
}

void Interpreter::enter_frame(FrameObject* frame) {
    _stack         = frame->_stack;
    _loop_stack    = frame->_loop_stack;
    _locals_table  = frame->_locals;
    _fast_locals   = frame->_fast_locals;
    _consts        = frame->_consts;
    _names         = frame->_names;
    _globals_table = frame->_globals;

    frame->set_sender(_top_frame);
    _top_frame     = frame;

    _codes         = frame->_codes;
    _pc            = 0;
    _code_length   = _codes->_bytecodes->length();
    _bytecodes     = _codes->_bytecodes->value();
}

void Interpreter::leave_last_frame() {
    if (!_top_frame->sender()) {
        //delete _top_frame;
        _top_frame = NULL;
        return;
    }

    HiObject* ret_value = _stack->pop();
    FrameObject* temp = _top_frame;
    _top_frame     = _top_frame->sender();
    _codes         = _top_frame->_codes;
    _pc            = _top_frame->get_pc();
    _code_length   = _codes->_bytecodes->length();
    _bytecodes     = _codes->_bytecodes->value();

    _stack         = _top_frame->_stack;
    _loop_stack    = _top_frame->_loop_stack;
    _locals_table  = _top_frame->_locals;
    _fast_locals   = _top_frame->_fast_locals;
    _consts        = _top_frame->_consts;
    _names         = _top_frame->_names;
    _globals_table = _top_frame->_globals;

    if (_top_frame->with_ret_value()) {
        _stack->push(ret_value);
    }

    delete temp;
}

void Interpreter::eval_code() {
    _pc = 0;
    bool x86 = false;
    HiObject* v, * w, * u;
    FunctionObject* fo;
    ArrayList<HiObject*>* args = NULL;

    while (_pc < _code_length) {
        unsigned char op_code = _bytecodes[_pc++];
        bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

        int op_arg = -1;
        if (has_argument) {
            op_arg = x86 ? (_bytecodes[_pc++] & 0xFF) : 
                ((_bytecodes[_pc++] & 0xFF) + ((_bytecodes[_pc++] & 0xFF) << 8));
        }

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

            case ByteCode::BUILD_LIST:
                v = new HiList();
                while (op_arg--) {
                    ((HiList*)v)->inner_list()->add(_stack->pop());
                }
                _stack->push(v); 
                break;

            case ByteCode::BUILD_MAP:
                v = new HiDict();
                _stack->push(v);
                break;

            case ByteCode::BUILD_CLASS:
                v = _stack->pop();
                u = _stack->pop();
                w = _stack->pop();
                v = Klass::create_klass(v, u, w);
                _stack->push(v);
                break;

            case ByteCode::LOAD_CONST:
                _stack->push(_consts->get(op_arg));
                break;

            case ByteCode::LOAD_FAST:
                _stack->push(_fast_locals->get(op_arg));
                break;

            case ByteCode::LOAD_GLOBAL:
                v = _names->get(op_arg);
                _stack->push(_globals_table->get(v));
                break;

            case ByteCode::LOAD_NAME:
                v = _names->get(op_arg);
                w = _locals_table->get(v);
                if (w != 0) {
                    _stack->push(w);
                    break;
                }

                w = _globals_table->get(v);
                if (w != 0) {
                    _stack->push(w);
                    break;
                }

                w = _builtins->get(v);
                if (w != 0) {
                    _stack->push(w);
                    break;
                }

                _stack->push(Universe::HiNone);
                break;

            case ByteCode::LOAD_ATTR:
                v = _stack->pop();
                w = _names->get(op_arg);
                _stack->push(v->getattr(w));
                break;

            case ByteCode::LOAD_LOCALS:
                _stack->push(new HiDict(_locals_table));
                break;


            case ByteCode::PRINT_ITEM:
                v = _stack->pop();
                v->print();
                break;

            case ByteCode::PRINT_NEWLINE:
                printf("\n");
                break;

            case ByteCode::STORE_FAST:
                _fast_locals->set(op_arg, _stack->pop());
                break;

            case ByteCode::STORE_GLOBAL:
                v = _names->get(op_arg);
                _globals_table->put(v, _stack->pop());
                break;

            case ByteCode::STORE_NAME:
                v = _names->get(op_arg);
                _locals_table->put(v, _stack->pop());
                break;

            case ByteCode::STORE_MAP:
                w = _stack->pop();
                u = _stack->pop();
                v = _stack->top();
                ((HiDict*)v)->put(u, w);
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
                leave_last_frame();
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
                fo = new FunctionObject(v, _globals_table);
                if (op_arg > 0) {
                    args = new ArrayList<HiObject*>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, _stack->pop());
                    }
                }
                fo->set_default(args);

                if (args != NULL) {
                    delete args;
                    args = NULL;
                }

                _stack->push(fo);
                break;

            case ByteCode::CALL_FUNCTION:
                _top_frame->set_pc(_pc);
                if (op_arg > 0) {
                    args = new ArrayList<HiObject*>(op_arg);
                    args->resize(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, _stack->pop());
                    }
                }

                call_func(_stack->pop(), args);
                if (args != NULL) {
                    delete args;
                    args = NULL;
                }
                break;

            default:
                // do nothing
                break;
        }
    }
}

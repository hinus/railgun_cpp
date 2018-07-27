#include "runtime/generator.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/frameObject.hpp"
#include "code/interpreter.hpp"
#include "code/codeObject.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "memory/oopClosure.hpp"

GeneratorKlass* GeneratorKlass::instance = NULL;

GeneratorKlass::GeneratorKlass() {
}

GeneratorKlass* GeneratorKlass::get_instance() {
    if (instance == NULL)
        instance = new GeneratorKlass();

    return instance;
}

HiObject* GeneratorKlass::iter(HiObject* obj) {
    return obj;
}

HiObject* GeneratorKlass::next(HiObject* obj) {
    assert(obj->klass() == (Klass*) this);
    Generator* g = (Generator*) obj;
    return Interpreter::get_instance()->eval_generator(g);
}

size_t GeneratorKlass::size() {
    return sizeof(Generator);
}

void GeneratorKlass::oops_do(OopClosure* f, HiObject* obj) {
    Generator* g = (Generator*)obj;
    assert(g->klass() == (Klass*)this);

    f->do_array_list(&g->_consts);
    f->do_array_list(&g->_names);

    f->do_oop((HiObject**)&g->_globals);
    f->do_oop((HiObject**)&g->_locals);

    f->do_oop((HiObject**)&g->_fast_locals);
    f->do_oop((HiObject**)&g->_stack);

    f->do_oop((HiObject**)&g->_codes);
}

Generator::Generator(FunctionObject* func, ArrayList<HiObject*>* args) {
    _codes   = func->_func_code;
    _globals = func->_globals;
    _consts  = _codes->_consts;
    _names   = _codes->_names;

    _locals = new HiDict();
    _fast_locals = new HiList();

    if (func->_defaults) {
        int dft_cnt = func->_defaults->length();
        int argcnt  = _codes->_argcount;

        while (dft_cnt--) {
            _fast_locals->set(--argcnt, func->_defaults->get(dft_cnt));
        }
    }

    if (args) {
        for (int i = 0; i < args->length(); i++) {
            _fast_locals->set(i, args->get(i));
        }
    }

    _stack  = new HiList();
    _loop_stack  = new Stack<Block>();
    _pc    = 0;

    set_klass(GeneratorKlass::get_instance());
}


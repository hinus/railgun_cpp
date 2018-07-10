#include "runtime/nativeFunction.hpp"
#include "runtime/universe.hpp"

NativeFunctionKlass* NativeFunctionKlass::instance = NULL;

NativeFunctionKlass* NativeFunctionKlass::get_instance() {
	if (instance == NULL)
		instance = new NativeFunctionKlass();

	return instance;
}

void NativeFunctionKlass::print(HiObject* obj) {
    printf("native function : ");
    NativeFunction* nfo = static_cast<NativeFunction*>(obj);

    assert(nfo && nfo->klass() == (Klass*) this);
    nfo->_func_name->print();
}

NativeFunction::NativeFunction(HiString* name) : _func_name(name) {
    set_klass(NativeFunctionKlass::get_instance());
}

NativeFunction::NativeFunction(const char* name) {
    _func_name = new HiString(name);
    set_klass(NativeFunctionKlass::get_instance());
}

bool NativeFunction::equals(HiString* x) {
    if (_func_name->equal(x) == (HiObject*)Universe::HiTrue) {
        return true;
    }
    return false;
}

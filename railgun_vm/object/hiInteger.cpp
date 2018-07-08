#include "object/hiInteger.hpp"
#include <assert.h>

HiInteger::HiInteger(int x) {
    _value = x;
    set_klass(IntegerKlass::get_instance());
}

IntegerKlass* IntegerKlass::instance = NULL;

IntegerKlass* IntegerKlass::get_instance() {
	if (instance == NULL)
		instance = new IntegerKlass();

	return instance;
}

void IntegerKlass::print(HiObject* obj) {
	HiInteger* int_obj = (HiInteger*) obj;

	assert(int_obj && ((char *)int_obj->klass()) == ((char *)this));

	printf("%d", int_obj->value());
}

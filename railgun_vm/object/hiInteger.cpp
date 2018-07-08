#include "Object/hiInteger.hpp"

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

	if (!int_obj)
		return;

	printf("%d", int_obj->value());
}

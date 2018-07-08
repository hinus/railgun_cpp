#include "object/hiString.hpp"
#include <assert.h>

StringKlass* StringKlass::instance = NULL;

StringKlass* StringKlass::get_instance() {
	if (instance == NULL)
		instance = new StringKlass();

	return instance;
}

void StringKlass::print(HiObject* obj) {
	HiString* str_obj = (HiString*) obj;

	assert(str_obj && str_obj->klass() == (Klass*)this);

	printf("%d", str_obj->value());
}

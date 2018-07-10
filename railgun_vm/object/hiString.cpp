#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "runtime/universe.hpp"
#include <assert.h>

StringKlass* StringKlass::instance = NULL;

StringKlass* StringKlass::get_instance() {
	if (instance == NULL)
		instance = new StringKlass();

	return instance;
}

HiObject* StringKlass::equal(HiObject* x, HiObject* y) {
	HiString* sx = (HiString*) x;
	HiString* sy = (HiString*) y;

	assert(sx && sx->klass() == (Klass*)this);
	assert(sy && sy->klass() == (Klass*)this);

    if (sx->length() != sy->length())
        return Universe::HiFalse;

    for (int i = 0; i < sx->length(); i++) {
        if (sx->value()[i] != sy->value()[i])
            return Universe::HiFalse;
    }

    return Universe::HiTrue;
}

void StringKlass::print(HiObject* obj) {
	HiString* str_obj = (HiString*) obj;

	assert(str_obj && str_obj->klass() == (Klass*)this);

	for (int i = 0; i < str_obj->length(); i++) {
		printf("%c", str_obj->value()[i]);
	}
}

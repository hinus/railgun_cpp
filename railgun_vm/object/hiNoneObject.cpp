#include "object/hiNoneObject.hpp"
#include "runtime/universe.hpp"

HiNoneObject* HiNoneObject::instance = NULL;

HiNoneObject* HiNoneObject::get_instance() {
	if (instance == NULL)
		instance = new HiNoneObject();

	return instance;
}

HiObject* Universe::HiNone = HiNoneObject::get_instance();

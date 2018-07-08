#include "object/hiNoneObject.hpp"

HiNoneObject* HiNoneObject::instance = NULL;

HiNoneObject* HiNoneObject::get_instance() {
	if (instance == NULL)
		instance = new HiNoneObject();

	return instance;
}
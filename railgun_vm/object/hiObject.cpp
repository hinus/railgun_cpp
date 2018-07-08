#include "object/hiObject.hpp"
#include <assert.h>

void HiObject::print() {
	assert(_klass != NULL);

	_klass->print(this);
}

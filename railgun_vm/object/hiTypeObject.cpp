#include "object/hiTypeObject.hpp"
#include "object/hiNoneObject.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/universe.hpp"

HiObject* HiTypeObject::get_type_attr(HiObject* obj, HiString* name) {
    for (int i = 0; i < _methods_length; i ++) {
        if (_methods[i]->equals(name))
            return new MethodObject(obj, NULL, _methods[i]);
    }

	return Universe::HiNone;
}

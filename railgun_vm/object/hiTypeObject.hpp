#ifndef HI_TYPE_OBJECT_HPP
#define HI_TYPE_OBJECT_HPP

#include "object/hiString.hpp"
#include "runtime/nativeFunction.hpp"

class HiTypeObject : public HiObject {
private:
    int              _methods_length;
    NativeFunction** _methods;

public:
    HiTypeObject() : _methods(NULL), _methods_length(0) {}
    HiObject* get_type_attr(HiObject* obj, HiString* name);

    void set_methods(NativeFunction** methods, int len) {
        _methods_length = len;
        _methods = methods;
    }
};

#endif

#include "object/hiNoneObject.hpp"
#include "runtime/universe.hpp"
#include "memory/heap.hpp"

HiNoneObject* HiNoneObject::instance = NULL;

HiNoneObject* HiNoneObject::get_instance() {
    if (instance == NULL)
        instance = new HiNoneObject();

    return instance;
}

void* HiNoneObject::operator new(size_t size) {
    return Universe::heap->allocate(size);
}


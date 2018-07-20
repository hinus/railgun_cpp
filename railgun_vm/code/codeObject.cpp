#include "code/codeObject.hpp"
#include "runtime/universe.hpp"
#include "memory/heap.hpp"

void* CodeObject::operator new(size_t size) {
    return Universe::heap->allocate_meta(size);
}


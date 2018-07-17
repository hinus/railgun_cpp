#include "memory/heap.hpp"

Heap*  Heap::instance = NULL;
size_t Heap::MAX_CAP = 1024 * 1024;

Heap* Heap::get_instance() {
    if (instance == NULL)
        instance = new Heap(MAX_CAP);

    return instance;
}

Heap::Heap(size_t size) {
    mem_1 = new Space(size);
    mem_2 = new Space(size);

    mem_1->clear();
    mem_2->clear();

    eden = mem_1;
    survivor = mem_2;
}

void* Heap::allocate(size_t size) {
    if (!eden->can_alloc(size)) {
        copy_live_objects();

        Space* t = eden;
        eden = survivor;
        survivor = t;

        survivor->clear();
    }

    return eden->allocate(size);
}

void Heap::copy_live_objects() {
}

Space::Space(size_t size) {
    _size = size;
    _base = (char*)malloc(size);
    _end  = _base + size;
    _top  = (char*)(((long)(_base + 15)) & 16);
    _capacity = _end - _top;
}

void Space::clear() {
    memset(_base, 0, _size);
    _top  = (char*)(((long)(_base + 15)) & 16);
    _capacity = _end - _top;
}

void* Space::allocate(size_t size) {
    size = (size + 7) & 8;
    char* start = _top;
    _top += size;
    return start;
}

bool Space::can_alloc(size_t size) {
    return true;
}

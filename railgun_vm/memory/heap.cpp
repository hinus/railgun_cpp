#include "runtime/universe.hpp"
#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"

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
    metaspace = new Space(size * 2);

    mem_1->clear();
    mem_2->clear();
    metaspace->clear();

    eden = mem_1;
    survivor = mem_2;
}

Heap::~Heap() {
    if (mem_1) {
        delete mem_1;
        mem_1 = NULL;
    }

    if (mem_2) {
        delete mem_2;
        mem_2 = NULL;
    }

    if (metaspace) {
        delete metaspace;
        metaspace = NULL;
    }

    eden = NULL;
    survivor = NULL;
}

void* Heap::allocate(size_t size) {
    if (!eden->can_alloc(size)) {
        gc();
    }

    return eden->allocate(size);
}

void* Heap::allocate_meta(size_t size) {
    if (!metaspace->can_alloc(size)) {
        return NULL;
    }

    return metaspace->allocate(size);
}

void Heap::copy_live_objects() {
    ScavengeOopClosure* closure = new ScavengeOopClosure(eden, survivor);
    closure->scavenge();
    delete closure;
}

void Heap::gc() {
    copy_live_objects();

    Space* t = eden;
    eden = survivor;
    survivor = t;

    survivor->clear();
}

Space::Space(size_t size) {
    _size = size;
    _base = (char*)malloc(size);
    _end  = _base + size;
    _top  = (char*)(((long)(_base + 15)) & -16);
    _capacity = _end - _top;
}

Space::~Space() {
    if (_base) {
        free(_base);
        _base = 0;
    }

    _top = 0;
    _end = 0;
    _capacity = 0;
    _size = 0;
}

void Space::clear() {
    memset(_base, 0, _size);
    _top  = (char*)(((long)(_base + 15)) & -16);
    _capacity = _end - _top;
}

void* Space::allocate(size_t size) {
    size = (size + 7) & -8;
    char* start = _top;
    _top += size;
    //printf("after allocate %lx, _top is %p\n", size, _top);
    return start;
}

bool Space::can_alloc(size_t size) {
    return true;
}

bool Space::has_obj(HiObject* obj) {
    return (char*)obj >= _base && _end > (char*)obj;
}


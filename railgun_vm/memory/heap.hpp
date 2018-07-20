#ifndef MEMORY_HEAP_HPP
#define MEMORY_HEAP_HPP

#include <memory.h>
#include <malloc.h>

class Space {
friend class Heap;
private:
    char*  _base;
    char*  _top;
    char*  _end;
    size_t _size;
    size_t _capacity;

    Space(size_t size);
    ~Space();

public:
    void* allocate(size_t size);
    void clear();
    bool can_alloc(size_t size);
};

class Heap {
private:
    Space* mem_1;
    Space* mem_2;
    
    Space* eden;
    Space* survivor;

    Space* metaspace;

    Heap(size_t size);
    static Heap* instance;

public:
    static size_t MAX_CAP;
    static Heap* get_instance();

    ~Heap();

    void* allocate(size_t size);
    void* allocate_meta(size_t size);
    void copy_live_objects();
};

#endif

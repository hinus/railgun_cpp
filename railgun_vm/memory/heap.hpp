#ifndef MEMORY_HEAP_HPP
#define MEMORY_HEAP_HPP

#include <memory.h>
#include <malloc.h>

class Space {
private:
    char*  _base;
    char*  _top;
    char*  _end;
    size_t _size;
    size_t _capacity;

public:
    Space(size_t size);

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

    Heap(size_t size);

public:
    static size_t MAX_CAP;
    static Heap* instance;
    static Heap* get_instance();

    void* allocate(size_t size);
    void copy_live_objects();
};

#endif

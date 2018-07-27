#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

#include <stdio.h>

class OopClosure;

template <typename T>
class ArrayList {
private:
    int _length;
    T*  _array;
    int _size;

    void expand();

public:
    ArrayList(int n = 8);

    void add(T t);
    void insert(int index, T t);
    T    get(int index);
    void set(int index, T t);
    int  size();
    int  length();
    T    pop();

    void* operator new(size_t size);

    void oops_do(OopClosure* closure);
};

class HiObject;
typedef ArrayList<HiObject*>* ObjList;

class Klass;
typedef ArrayList<Klass*>* KlassList;

#endif

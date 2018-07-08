#include "util/arrayList.hpp"

template <typename T>
ArrayList<T>::ArrayList(int n) {
    _length = n;
    _size   = 0;
    _array  = new T[n];
}

template <typename T>
void ArrayList<T>::add(T t) {
    _array[_size++] = t;
}

class HiObject;
template class ArrayList<HiObject*>;

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

template <typename T>
T ArrayList<T>::get(int index) {
    return _array[index];
}

class HiObject;
template class ArrayList<HiObject*>;

class HiString;
template class ArrayList<HiString*>;

#include "util/arrayList.hpp"
#include <stdio.h>

template <typename T>
ArrayList<T>::ArrayList(int n) {
    _length = n;
    _size   = 0;
    _array  = new T[n];
}

template <typename T>
void ArrayList<T>::add(T t) {
    expand();
    _array[_size++] = t;
}

template <typename T>
void ArrayList<T>::expand() {
    if (_size >= _length) {
        T* new_array = new T[_length << 1];
        for (int i = 0; i < _length; i++) {
            new_array[i] = _array[i];
        }
        delete[] _array;
        _array = new_array;
		_length <<= 1;
        printf("expand an array!\n");
    }
}

template <typename T>
int ArrayList<T>::size() {
    return _size;
}

template <typename T>
int ArrayList<T>::length() {
    return _length;
}

template <typename T>
T ArrayList<T>::get(int index) {
    return _array[index];
}

template <typename T>
void ArrayList<T>::set(int index, T t) {
    _array[index] = t;
}

class HiObject;
template class ArrayList<HiObject*>;

class HiString;
template class ArrayList<HiString*>;

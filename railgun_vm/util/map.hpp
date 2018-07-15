#ifndef UTIL_MAP_HPP
#define UTIL_MAP_HPP

#include "runtime/universe.hpp"

class HiObject;

template <typename K, typename V>
class MapEntry {
public:
    K _k;
    V _v;
    
    MapEntry(const MapEntry<K, V>& entry);
    MapEntry(K k, V v) : _k(k), _v(v) {}
    MapEntry() : _k(0), _v(0) {}
};

template <typename K, typename V>
class Map {
private:
    MapEntry<K, V>* _entries;
    int _size;
    int _length;
    
    void expand();
public:
    Map();

    int  size() { return _size; }
    void put(K k, V v);
    V    get(K k);
    V    remove(K k);
    int  index(K k);
    MapEntry<K, V>* entries() { return _entries; }
};

template <typename K, typename V>
Map<K, V>::Map() {
    _entries = new MapEntry<K, V>[8];
    _length  = 8;
    _size    = 0;
}

template <typename K, typename V>
MapEntry<K, V>::MapEntry(const MapEntry<K, V>& entry) {
    _k = entry._k;
    _v = entry._v;
}

template <typename K, typename V>
void Map<K, V>::put(K k, V v) {
    for (int i = 0; i < _size; i++) {
        if (_entries[i]._k->equal(k) == (HiObject*)Universe::HiTrue) {
            _entries[i]._v = v;
            return;
        }
    }
    
    expand();
    _entries[_size++] = MapEntry<K, V>(k, v);
}

template <typename K, typename V>
V Map<K, V>::get(K k) {
    int i = index(k);
    if (i < 0)
        return 0;
    else
        return _entries[i]._v;
}

template <typename K, typename V>
int Map<K, V>::index(K k) {
    for (int i = 0; i < _size; i++) {
        if (_entries[i]._k->equal(k) == (HiObject*)Universe::HiTrue) {
            return i;
        }
    }

    return -1;
}

template <typename K, typename V>
void Map<K, V>::expand() {
    if (_size >= _length) {
        MapEntry<K, V>* new_entries = new MapEntry<K, V>[_length << 1];
        for (int i = 0; i < _size; i++) {
            new_entries[i] = _entries[i];
        }
        _length <<= 1;
        delete[] _entries;
        _entries = new_entries;
    }
}

template <typename K, typename V>
V Map<K, V>::remove(K k) {
    int i = index(k);

    if (k < 0)
        return 0; 

    V v = _entries[i]._v;
    _entries[i] = _entries[_size--];
    return v;
}

#endif

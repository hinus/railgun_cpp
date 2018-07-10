#include "util/map.hpp"

template <typename K, typename V>
Map<K, V>::Map() {
    _entries = new MapEntry<K, V>[8];
    _length  = 8;
    _size    = 0;
}

template <typename K, typename V>
void Map<K, V>::put(K k, V v) {
    for (int i = 0; i < _size; i++) {
        if (_entries[i]._k->equal(k)) {
            _entries[i]._v = v;
            return;
        }
    }
    
    //expand();
    _entries[_size++] = MapEntry(k, v);
}

template <typename K, typename V>
V get(K k) {
    for (int i = 0; i < _size; i++) {
        if (_entries[i]._k->equal(k)) {
            return _entries[i]._v;
        }
    }
    
    return NULL;
}

template <typename K, typename V>
V remove(K k) {
    return NULL;
}

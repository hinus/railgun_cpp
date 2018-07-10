#ifndef UTIL_MAP_HPP
#define UTIL_MAP_HPP

template <typename K, typename V>
class MapEntry {
public:
    K _k;
    V _v;
    
    MapEntry(K k, V v) : _k(k), _v(v) {}
};

template <typename K, typename V>
class Map {
private:
    MapEntry<K, V>* _entries;
    
public:
    Map();
    void put(K k, V v);
    V    get(K k);
    V    remove(K k);
};

#endif
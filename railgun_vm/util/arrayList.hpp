#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

template <typename T>
class ArrayList {
private:
    int _length;
    T*  _array;
    int _size;

public:
    ArrayList(int n = 16);

    void add(T t);
	T    get(int index);
    void set(int index, T t);
};

#endif

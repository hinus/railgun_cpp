#ifndef _STACK_HPP
#define _STACK_HPP

template<typename V>
class Stack {
private:
    V* vector;
    int _len;
public:
    Stack(int n = 16) {
        vector = new V[n];
        _len = 0;
    }

    ~Stack() {
        delete[] vector;
        _len = 0;
    }

    void push(V v) {
        vector[_len++] = v;
    }

    V pop() {
        return vector[--_len];
    }

    V top() {
        return vector[_len - 1];
    }

    int len() {
        return _len;
    }
};

#endif

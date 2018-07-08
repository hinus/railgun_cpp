#include "klass/klass.hpp"

Klass* Klass::allocate_instance_klass() {
    return new Klass();
}
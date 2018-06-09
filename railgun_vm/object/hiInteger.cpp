#include "Object/hiInteger.hpp"

HiInteger::HiInteger(int x) {
    _value = x;
    _klass = Universe::_int_klass;
}

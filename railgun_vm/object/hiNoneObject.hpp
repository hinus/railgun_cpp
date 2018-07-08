#ifndef HI_NONE_OBJECT_HPP
#define HI_NONE_OBJECT_HPP

#include "object/hiObject.hpp"

class HiNoneObject : public HiObject {
private:
	HiNoneObject() {}
	static HiNoneObject * instance;

public:
	static HiNoneObject* get_instance();
};

#endif
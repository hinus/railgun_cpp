#ifndef HI_FUNCTION_OBJECT_HPP
#define HI_FUNCTION_OBJECT_HPP

#include "code/codeObject.hpp"

class FunctionObject : public HiObject {
friend class Interpreter;
friend class FrameObject;

private:
    CodeObject* _func_code;
    HiString*   _func_name;
    ArrayList<HiObject*>* _consts;
    ArrayList<HiObject*>* _globals;
    ArrayList<HiObject*>* _defaults;

public:
    FunctionObject(HiObject* code_object, ArrayList<HiObject*>* globals);
	
	void set_default(ArrayList<HiObject*>* defaults);
};

#endif

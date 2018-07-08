#ifndef CODE_OBJECT_HPP
#define CODE_OBJECT_HPP

#include <iostream>

#include "object/hiObject.hpp"
#include "util/arrayList.hpp"
#include "object/hiString.hpp"

class CodeObject : public HiObject {
public:
	int _argcount;
	int _nlocals;
	int _stack_size;
	int _flag;

	char* _bytecodes;
	ArrayList<HiObject*>*  _names;
	ArrayList<HiObject*>*  _consts;
	ArrayList<HiObject*>*  _var_names;

	ArrayList<HiObject*>*  _free_vars;
	ArrayList<HiObject*>*  _cell_vars;

	HiString* _module_name;
	HiString* _file_name;

	int _lineno;
	char* _notable;

	CodeObject(int argcount, int nlocals, int stacksize, int flag, char* bytecodes,
                      ArrayList<HiObject*>* consts, ArrayList<HiObject*>* names, ArrayList<HiObject*>* varnames, 
					  ArrayList<HiObject*>* freevars, ArrayList<HiObject*>* cellvars,
					  HiString* file_name, HiString* module_name, int lineno, char* notable):
		_argcount(argcount),
		_nlocals(nlocals),
		_stack_size(stacksize),
		_flag(flag),
		_bytecodes(bytecodes),
		_consts(consts),
		_names(names),
		_var_names(varnames),
		_free_vars(freevars),
		_cell_vars(cellvars),
		_file_name(file_name),
		_module_name(module_name),
		_lineno(lineno),
		_notable(notable){
			//std::cout << "lnotable length is " << strlen(notable) << std::endl;
			std::cout << "file name is ";
			file_name->print();
			std::cout << std::endl;

			std::cout << "module name is ";
			module_name->print();
			std::cout << std::endl;
		}
};

#endif
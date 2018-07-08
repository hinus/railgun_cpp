#ifndef BINARY_FILE_PARSER_HPP
#define BINARY_FILE_PARSER_HPP

#include "util/bufferedInputStream.hpp"
#include "util/arrayList.hpp"
#include "object/hiObject.hpp"
#include "code/codeObject.hpp"

class BinaryFileParser {
private:
	BufferedInputStream* file_stream;
	int cur;
	
public:
	BinaryFileParser(BufferedInputStream* stream);
	
public:
	CodeObject* parse();
    CodeObject* get_code_object();
    char*       get_byte_codes();
	int         get_int();

	ArrayList<HiObject*>*   get_consts();
	ArrayList<HiObject*>*   get_tuple();
};

#endif
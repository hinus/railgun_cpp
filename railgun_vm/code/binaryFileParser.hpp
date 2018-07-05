#ifndef BINARY_FILE_PARSER_HPP
#define BINARY_FILE_PARSER_HPP

#include "util/bufferedInputStream.hpp"
#include "code/codeObject.hpp"

class BinaryFileParser {
private:
	BufferedInputStream* file_stream;
	int cur;
	
public:
	BinaryFileParser(BufferedInputStream* stream);
	
public:
	CodeObject* parse();
	int get_int();
};

#endif
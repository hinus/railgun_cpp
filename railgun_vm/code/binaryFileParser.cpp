#include <iostream>

#include "code/binaryFileParser.hpp"

BinaryFileParser::BinaryFileParser(BufferedInputStream* buf_file_stream) {
	file_stream = buf_file_stream;
}

CodeObject* BinaryFileParser::parse() {
	int magic_number = file_stream->read_int();
	std::cout << magic_number << std::endl;
}
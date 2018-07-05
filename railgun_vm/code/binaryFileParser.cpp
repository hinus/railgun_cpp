#ifndef BINARY_FILE_PARSER_HPP
#define BINARY_FILE_PARSER_HPP

#include "util/bufferedInputStream"

BinaryFileParser::BinaryFileParser(BufferedInputStream& file_stream) {
	this.file_stream = file_stream;
}

CodeObject* BinaryFileParser::parse() {
	int magic_number = file_stream.read_int();
	cout << magic_number << endl;
}

#endif
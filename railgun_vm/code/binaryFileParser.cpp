#include <iostream>
#include <assert.h>

#include "code/binaryFileParser.hpp"

BinaryFileParser::BinaryFileParser(BufferedInputStream* buf_file_stream) {
	file_stream = buf_file_stream;
}

CodeObject* BinaryFileParser::parse() {
	int magic_number = file_stream->read_int();
	std::cout << magic_number << std::endl;
	int moddate = file_stream->read_int();
	std::cout << moddate << std::endl;

    char object_type = file_stream->read();

    if (object_type == 'c') {
        return get_code_object();
    }

    return NULL;
}

CodeObject* BinaryFileParser::get_code_object() {
    int argcount  = file_stream->read_int();
    std::cout << argcount << std::endl;
    int nlocals   = file_stream->read_int();
    int stacksize = file_stream->read_int();
    int flags     = file_stream->read_int();
    std::cout << flags << std::endl;

    char* byte_codes = get_byte_codes();
    ArrayList<HiObject*>* consts = get_consts();

    return NULL;
}

char* BinaryFileParser::get_byte_codes() {
    assert(file_stream->read() == 's');

    int length = file_stream->read_int();
    std::cout << "code length is " << length << std::endl;
    char* code = new char[length];

    for (int i = 0; i < length; i++) {
        code[i] = file_stream->read();
    }

    return code;
}

ArrayList<HiObject*>* BinaryFileParser::get_consts() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }

    file_stream->unread();
    return NULL;
}

ArrayList<HiObject*>* BinaryFileParser::get_tuple() {
    int length = file_stream->read_int();

    ArrayList<HiObject*>* list = new ArrayList<HiObject*>(length);
    for (int i = 0; i < length; i++) {
        char obj_type = file_stream->read();

        switch (obj_type) {
        case 'c':
            std::cout << "got a code object." << std::endl;
            list->add(get_code_object());
            break;
        }
    }

	return NULL;
}

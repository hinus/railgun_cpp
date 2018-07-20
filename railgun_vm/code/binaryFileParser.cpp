#include <iostream>
#include <assert.h>

#include "code/binaryFileParser.hpp"
#include "object/hiNoneObject.hpp"
#include "object/hiList.hpp"

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
        CodeObject* result = get_code_object();
        std::cout << "parse ok!" << std::endl;
        return result;
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

    HiString* byte_codes = get_byte_codes();
    ArrayList<HiObject*>* consts     = get_consts();
    ArrayList<HiObject*>* names      = get_names();
    ArrayList<HiObject*>* var_names  = get_var_names();
    ArrayList<HiObject*>* free_vars  = get_free_vars();
    ArrayList<HiObject*>* cell_vars  = get_cell_vars();

    HiString* file_name   = get_file_name();
    HiString* module_name = get_name();
    int begin_line_no     = file_stream->read_int();
    HiString* lnotab      = get_no_table();

    return new CodeObject(argcount, nlocals, stacksize, flags, byte_codes,
        consts, names, var_names, free_vars, cell_vars, file_name, module_name,
        begin_line_no, lnotab);
}

HiString* BinaryFileParser::get_string() {
    int length = file_stream->read_int();
    char* str_value = new char[length];

    for (int i = 0; i < length; i++) {
        str_value[i] = file_stream->read();
    }

    HiString* s = new HiString(str_value, length);
    delete[] str_value;

    return s;
}

HiString* BinaryFileParser::get_name() {
    char ch = file_stream->read();

    if (ch == 's') {
        return get_string();
    }
    else if (ch == 't') {
        HiString* str = get_string();
        _string_table.add(str);
        return str;
    }
    else if (ch == 'R') {
        return _string_table.get(file_stream->read_int());
    }
    
    return NULL;
}

HiString* BinaryFileParser::get_file_name() {
    return get_name();
}

HiString* BinaryFileParser::get_byte_codes() {
    assert(file_stream->read() == 's');

    return get_string();
}

HiString* BinaryFileParser::get_no_table() {
    char ch = file_stream->read();
    
    if (ch != 's' && ch != 't') {
        file_stream->unread();
        return NULL;
    }

    return get_string();
}

ArrayList<HiObject*>* BinaryFileParser::get_consts() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }

    file_stream->unread();
    return NULL;
}

ArrayList<HiObject*>* BinaryFileParser::get_names() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }

    file_stream->unread();
    return NULL;
}

ArrayList<HiObject*>* BinaryFileParser::get_var_names() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }

    file_stream->unread();
    return NULL;
}

ArrayList<HiObject*>* BinaryFileParser::get_free_vars() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }

    file_stream->unread();
    return NULL;
}

ArrayList<HiObject*>* BinaryFileParser::get_cell_vars() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }

    file_stream->unread();
    return NULL;
}

ArrayList<HiObject*>* BinaryFileParser::get_tuple() {
    int length = file_stream->read_int();
    HiString * str;

    ArrayList<HiObject*>* list = new ArrayList<HiObject*>(length);
    for (int i = 0; i < length; i++) {
        char obj_type = file_stream->read();

        switch (obj_type) {
        case 'c':
            std::cout << "got a code object." << std::endl;
            list->add(get_code_object());
            break;
        case 'i':
            list->add(new HiInteger(file_stream->read_int()));
            break;
        case 'N':
            list->add(HiNoneObject::get_instance());
            break;
        case 't':
            str = get_string();
            list->add(str);
            _string_table.add(str);
            break;
        case 's':
            list->add(get_string());
            break;
        case 'R':
            list->add(_string_table.get(file_stream->read_int()));
            break;
        case '(':
            list->add(new HiList(get_tuple()));
            break;
        }
    }

    return list;
}

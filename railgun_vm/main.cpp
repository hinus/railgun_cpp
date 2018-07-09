#include <iostream>

#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "code/codeObject.hpp"
#include "code/interpreter.hpp"

int main(int argc, char** argv) {
    BufferedInputStream input_stream(argv[1]);
	BinaryFileParser parser(&input_stream);
	Interpreter interpreter;

	interpreter.run(parser.parse());

	return 0;
}

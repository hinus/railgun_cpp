#include <iostream>

#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "code/codeObject.hpp"
#include "code/interpreter.hpp"
#include "memory/heap.hpp"
#include "runtime/universe.hpp"

int main(int argc, char** argv) {
    Universe::genesis();

    BufferedInputStream input_stream(argv[1]);
    BinaryFileParser parser(&input_stream);

    Interpreter interpreter;
    interpreter.run(parser.parse());
    input_stream.close();

    Universe::heap->copy_live_objects();

    Universe::destroy();

    return 0;
}

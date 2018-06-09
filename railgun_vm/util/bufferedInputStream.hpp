#include "bufferedInputStream.hpp"

BufferedInputStream::BufferedInputStream(char const* filename) {
    inputStream.open(filename, std::ios::binary);
    index = 0;
    inputStream.read(szBuffer, sizeof(szBuffer));
}

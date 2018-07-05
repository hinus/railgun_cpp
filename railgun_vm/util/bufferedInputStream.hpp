#include <iostream>
#include <fstream>

#define BUFFER_LEN 256

class BufferedInputStream {
private:
    std::ifstream inputStream;
    char szBuffer[BUFFER_LEN];
    unsigned short index;

public:
    BufferedInputStream(char const* filename);

    char read() {
        if (index < BUFFER_LEN)
            return szBuffer[index++];
        else {
            index = 0;
            inputStream.read(szBuffer, BUFFER_LEN * sizeof(char));
            return szBuffer[index++];
        }
    }

    int read_int() {
        int b1 = read();
        int b2 = read();
        int b3 = read();
        int b4 = read();

        return b4 << 24 | b3 << 16 | b2 << 8 | b1;
    }

    void close() {
        inputStream.close();
    }
};

#include "bit_reader.h"
#include <exception>

BitReader::
BitReader(std::ifstream& inputStream) : stream(inputStream)
{
    
}

void
BitReader::
refreshBuffer()
{
    if (stream.eof()) throw std::exception("end of file");

    buffer = 0;
    int i = sizeof(byte);
    stream.read((char*)&buffer, sizeof(byte));
    bufferPosition = 0;
}

bool
BitReader::
readBit()
{
    if (bufferPosition % 8 == 0) {
        refreshBuffer();
    }

    bool output =  (buffer >> (7 - bufferPosition)) & 1;

    bufferPosition++;

    return output;
}

wchar_t
BitReader::
readWChar()
{
    wchar_t output = 0;

    for (int i = 15; i >= 0; --i) {
        int b = readBit() << i;
        output |= b;
    }

    return output;
}

bool
BitReader::
eof()
{
    return stream.eof();
}
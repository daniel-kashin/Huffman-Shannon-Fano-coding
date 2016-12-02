#ifndef BIT_WRITER_CPP
#define BIT_WRITER_CPP

#include "bit_writer.h"

#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>

void
BitWriter::
clearBuffer()
{
    buffer = 0;
    bufferBitSize = 0;
}

BitWriter::
BitWriter(std::ofstream& inputStream) : stream(inputStream)
{
    clearBuffer();
}

void
BitWriter::
writeBit(int i)
{
    buffer |= i << (7 - bufferBitSize);
    bufferBitSize++;

    if (bufferBitSize == 8) {
        stream.write((char*)&buffer, sizeof(buffer));
        clearBuffer();
    }
}

void
BitWriter::
writeWChar(wchar_t wchar)
{
    for (int i = 15; i >= 0; --i) {
        bool b = (wchar >> i) & 1;
        writeBit(b);
    }
}

void
BitWriter::
writeBits(const std::vector<bool>* bits)
{
    for (bool b : *bits) {
        writeBit(b);
    }
}

void
BitWriter::
forceWrite()
{
    if (bufferBitSize > 0) {
        stream << buffer;
        clearBuffer();
    }
}

#endif
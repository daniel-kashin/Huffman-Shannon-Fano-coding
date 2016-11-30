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
BitWriter(const char* filename)
{
    this->filename = filename;
    
    stream = std::ofstream(filename, std::ios::binary);
    
    if (!stream.is_open()) {
        std::cout << "Невозможно открыть файл: ";
        std::cout << filename;
    }

    clearBuffer();
}

BitWriter::
~BitWriter()
{
    stream.close();
}

void
BitWriter::
writeBit(int i)
{
    try {
        buffer |= i << (7 - bufferBitSize);
        bufferBitSize++;

        if (bufferBitSize == BITS_PER_BYTE) {
            stream << buffer;
            clearBuffer();
        }
    }
    catch (...) {
        std::cout << "Unable to open file.";
        std::cout << "\n";
    }
}

void
BitWriter::
writeWChar(wchar_t wchar)
{
    std::bitset<BITS_PER_BYTE * 2> bitset(wchar);
    for (int i = 0; i < BITS_PER_BYTE * 2; ++i) {
        writeBit(bitset[i]);
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
    try {
        stream << buffer;
        clearBuffer();
    }
    catch (...) {
        std::cout << "Unable to open file.";
        std::cout << "\n";
    }
}

#endif
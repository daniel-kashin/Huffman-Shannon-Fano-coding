#ifndef BIT_WRITER_H
#define BIT_WRITER_H

#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>

using byte = unsigned char;
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<byte>::digits;


class BitWriter
{
private:
    const char* filename;
    std::ofstream stream;
    byte buffer;
    byte bufferBitSize;

    void clearBuffer();

public:

    BitWriter(const char* filename);

    ~BitWriter();

    void writeBit(const int i);

    void writeWChar(const wchar_t wchar);

    void writeBits(const std::vector<bool>* bist);

    void forceWrite();

};

#endif
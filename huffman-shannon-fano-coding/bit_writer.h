#pragma once

#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>

using byte = unsigned char;


class BitWriter
{
private:
    std::ofstream& stream;
    byte buffer;
    byte bufferBitSize;

    void clearBuffer();

public:

    BitWriter(std::ofstream& stream);

    void writeBit(const int i);

    void writeWChar(const wchar_t wchar);

    void writeBits(const std::vector<bool>* bist);

    void forceWrite();
};
#pragma once

#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>

using byte = unsigned char;

class BitReader
{
private:
    std::ifstream& stream;
    byte buffer = 0;
    byte bufferPosition = 0;

    void refreshBuffer();

public:

    BitReader(std::ifstream& inputStream);

    bool readBit();

    wchar_t readWChar();

    bool eof();
};
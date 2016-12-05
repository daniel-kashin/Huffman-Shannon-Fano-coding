#pragma once

#include <string>

extern long long globalCounter;

void
encodeWithHuffman();

void
decodeWithHuffman();

void
encodeWithShannonFano();

void
decodeWithShannonFano();

void
encode(bool isHuffman);

void
decode(bool isHuffman);


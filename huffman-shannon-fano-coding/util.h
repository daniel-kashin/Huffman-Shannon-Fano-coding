#pragma once

#include "bit_writer.h"
#include "heap.h"
#include "bit_reader.h"

#include <unordered_map>
#include <vector>

class Util {
public:
    Util() {}

    // util 

    static bool hasEnding(std::string const &fullString, std::string const &ending);

    static const std::string readFileNameFromConsole(const char* message, const char* fileType);

    static std::vector<std::pair<wchar_t, int>> getVector(std::unordered_map<wchar_t, int> map);

    static void changeFileNameFormat(const std::string& oldName, const std::string& newFormat, std::string* newName);


    // encoding

    static std::unordered_map<wchar_t, int> getFileCharFrequencies(std::string&  filename);

    static void fillHuffmanCodes(Heap::Node* root, std::unordered_map<wchar_t, std::vector<bool>*>* huffmanCodes);

    static Heap::Node* buildHuffmanTree(std::vector<std::pair<wchar_t, int>> frequences);

    static void encodeFile(std::unordered_map<wchar_t, std::vector<bool>*>* huffmanCodes,
        std::string& inputFileName, BitWriter* writer);

    static void encodeNode(Heap::Node* node, BitWriter* writer);

    static void fillHuffmanCodesFromRoot(Heap::Node* root, int* buffer, int bufferTop,
        std::unordered_map<wchar_t, std::vector<bool>*>* output);


    // decoding

    static int decodeFileLength(std::ifstream* stream);

    static Heap::Node* decodeNode(BitReader* bitReader);

    static void decodeFile(BitReader* bitReader, std::string& outputFileName, Heap::Node* root, int length);
};
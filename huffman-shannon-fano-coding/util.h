#ifndef UTIL_H
#define UTIL_H

#include "bit_writer.h"
#include "heap.h"

#include <map>
#include <vector>

class Util {
public:
    Util() {}

    static void encodeFile(std::map<wchar_t, std::vector<bool>*>* huffmanCodes,
         char* inputFileName, char* outputFileName, BitWriter* writer);

    static void encodeNode(Heap::Node* node, BitWriter* writer);

    static Heap::Node* buildHuffmanTree(std::vector<std::pair<wchar_t, int>> frequences);

    static std::map<wchar_t, int> getFileCharFrequencies(char* filename);

    static std::vector<std::pair<wchar_t, int>> getVector(std::map<wchar_t, int> map);

    static std::string arrToString(int arr[], int n);

    static void handleNode(Heap::Node* root, int* arr, int top,
        std::map<wchar_t, std::vector<bool>*>* output);

    static void fillHuffmanCodes(Heap::Node* root, std::map<wchar_t, std::vector<bool>*>* huffmanCodes);
};

#endif UTIL_H
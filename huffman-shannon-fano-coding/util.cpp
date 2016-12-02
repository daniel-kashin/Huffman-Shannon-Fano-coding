#include "util.h"
#include "heap.h"
#include "bit_writer.h"

#include <fstream>
#include <codecvt>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <locale>
#include <bitset>

// util functions

bool
Util::
hasEnding(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}



const std::string
Util::
readFileNameFromConsole(const char* message, const char* fileType)
{
    std::cout << message << std::endl;

    std::string filename;

    bool wrongInput = true;
    while (wrongInput) {
        std::cin >> filename;
        wrongInput = !hasEnding(filename, fileType);
        if (wrongInput) {
            std::cout << "Wrong filename paramether." << std::endl;
            filename = "";
        }
    }

    return filename;
}

void
Util::
changeFileNameFormat(const std::string& oldName, const std::string& newFormat, std::string* newName)
{
    int length = static_cast<int>(oldName.length());
    for (int i = 0; i < length && oldName[i] != '.'; ++i) {
        *newName += oldName[i];
    }
    *newName += newFormat;
}

std::vector<std::pair<wchar_t, int>>
Util::
getVector(std::unordered_map<wchar_t, int> map)
{
    auto vector = std::vector<std::pair<wchar_t, int>>();

    for (std::pair<wchar_t, int> pair : map) {
        vector.push_back(pair);
    }

    return vector;
}


// encoding functions

void
Util::
encodeNode(Heap::Node* node, BitWriter* writer) {
    if (node != nullptr) {
        if (node->isLeaf()) {
            writer->writeBit(1);
            writer->writeWChar(node->value);
        }
        else {
            writer->writeBit(0);
            encodeNode(node->left, writer);
            encodeNode(node->right, writer);
        }
    }
}

void
Util::
encodeFile(std::unordered_map<wchar_t, std::vector<bool>*>* huffmanCodes,
    std::string& inputFileName, BitWriter* writer)
{
    // initialize locale and stream
    std::locale locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wifstream stream(inputFileName, std::ios::binary);
    stream.imbue(locale);

    wchar_t current;

    // go through the file and count all the characters
    if (stream.is_open()) {
        while (!stream.eof()) {
            if (stream.get(current)) {
                writer->writeBits(huffmanCodes->at(current));
            }
        }

        stream.close();
    }

    writer->forceWrite();
}

void
Util::
fillHuffmanCodes(Heap::Node* root, std::unordered_map<wchar_t, std::vector<bool>*>* huffmanCodes)
{
    const int top = 0;
    const int maxTreeHeight = 50;
    int *buffer = new int[maxTreeHeight];
    Util::fillHuffmanCodesFromRoot(root, buffer, top, huffmanCodes);
}

void
Util::
fillHuffmanCodesFromRoot(Heap::Node* root, int* buffer, int bufferTop,
    std::unordered_map<wchar_t, std::vector<bool>*>* huffmanCodes)
{
    if (root != nullptr) {
        if (root->left) {
            buffer[bufferTop] = 0;
            fillHuffmanCodesFromRoot(root->left, buffer, bufferTop + 1, huffmanCodes);
        }

        if (root->right) {
            buffer[bufferTop] = 1;
            fillHuffmanCodesFromRoot(root->right, buffer, bufferTop + 1, huffmanCodes);
        }

        if (root->isLeaf()) {
            (*huffmanCodes)[root->value] = new std::vector<bool>(buffer, buffer + bufferTop);
        }
    }
}

std::unordered_map<wchar_t, int>
Util::
getFileCharFrequencies(std::string& filename)
{
    // initialize locale and stream
    std::locale locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wifstream stream(filename, std::ios::binary);
    stream.imbue(locale);

    // create the container for ccounting 
    std::unordered_map<wchar_t, int> frequencies;

    wchar_t current;

    // go through the file and count all the characters
    if (stream.is_open()) {
        while (!stream.eof()) {
            if (stream.get(current)) {
                frequencies[current]++;
            }
        }

        stream.close();
    }

    return frequencies;
}

Heap::Node*
Util::buildHuffmanTree(std::vector<std::pair<wchar_t, int>> frequences) {
    Heap::Node *left;
    Heap::Node *right;
    Heap::Node *top;

    Heap* heap = new Heap(frequences, static_cast<int>(frequences.size()));

    while (heap->currentSize > 1)
    {
        left = heap->extractMin();
        right = heap->extractMin();

        top = new Heap::Node(0, left->frequency + right->frequency, left, right);
        heap->insert(top);
    }

    return heap->extractMin();
};


// decoding functions

int
Util::
decodeFileLength(std::ifstream* stream)
{
    int length = 0;
    stream->read((char*)&length, sizeof(length));
    return length;
}

Heap::Node*
Util::
decodeNode(BitReader* bitReader)
{
    if (bitReader->readBit() == 1) {
        return new Heap::Node(bitReader->readWChar(), nullptr, nullptr);
    } else {
        Heap::Node* leftChild = decodeNode(bitReader);
        Heap::Node* rightChild = decodeNode(bitReader);
        return new Heap::Node(0, leftChild, rightChild);
    }
}

void
Util::
decodeFile(BitReader* bitReader, std::string& outputFileName, Heap::Node* root, int length)
{
    std::locale locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wofstream stream = std::wofstream(outputFileName, std::ios::binary);
    stream.imbue(locale);

    int position = 0;
    Heap::Node node = *root;
    while (position < length && !bitReader->eof()) {
        if (node.value != 0) {
            stream.write((wchar_t*)&node.value, 1);
            node = *root;
            position++;
        } else if (bitReader->readBit()) {
            node = *node.right;
        } else {
            node = *node.left;
        }
    }

}
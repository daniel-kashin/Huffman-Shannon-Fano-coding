#include "main.h"
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
readFileNameFromConsole(const std::string& fileType)
{
    std::string filename;

    bool wrongInput = true;
    while (wrongInput) {
        std::cin >> filename;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
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
    globalCounter += 1;
    if (node != nullptr) {
        globalCounter += 1;
        if (node->isLeaf()) {
            writer->writeBit(1);
            writer->writeWChar(node->value);
            globalCounter += 1;
        }
        else {
            writer->writeBit(0);
            encodeNode(node->left, writer);
            encodeNode(node->right, writer);
            globalCounter += 2;
        }
    }
}

void
Util::
encodeFile(std::unordered_map<wchar_t, std::vector<bool>*>* huffmanCodes,
    std::string& inputFileName, BitWriter* writer)
{
    std::locale locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wifstream stream(inputFileName, std::ios::binary);
    stream.imbue(locale);

    wchar_t current;

    globalCounter += 2;
    if (stream.is_open()) {
        while (!stream.eof()) {
            if (stream.get(current)) {
                writer->writeBits(huffmanCodes->at(current));
                globalCounter += 4;
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
    globalCounter += 4;

    Util::fillHuffmanCodesFromRoot(root, buffer, top, huffmanCodes);
}

void
Util::
fillHuffmanCodesFromRoot(Heap::Node* root, int* buffer, int bufferTop,
    std::unordered_map<wchar_t, std::vector<bool>*>* huffmanCodes)
{
    globalCounter += 1;
    if (root != nullptr) {
        if (root->left) {
            buffer[bufferTop] = 0;
            fillHuffmanCodesFromRoot(root->left, buffer, bufferTop + 1, huffmanCodes);
            globalCounter += 5;
        }

        if (root->right) {
            globalCounter += 5;
            buffer[bufferTop] = 1;
            fillHuffmanCodesFromRoot(root->right, buffer, bufferTop + 1, huffmanCodes);
        }

        if (root->isLeaf()) {
            globalCounter += 5;
            (*huffmanCodes)[root->value] = new std::vector<bool>(buffer, buffer + bufferTop);
        }
    }
}

std::unordered_map<wchar_t, int>
Util::
getFileCharFrequencies(std::string& filename)
{
    std::locale locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wifstream stream(filename, std::ios::binary);
    stream.imbue(locale);

    std::unordered_map<wchar_t, int> frequencies;

    wchar_t current;

    globalCounter += 2;

    globalCounter += 1;
    if (stream.is_open()) {
        while (!stream.eof()) {
            globalCounter += 4;

            if (stream.get(current)) {
                frequencies[current]++;
                globalCounter += 2;
            }
        }
        globalCounter += 1;
        stream.close();
    }

    globalCounter += 1;
    return frequencies;
}

Heap::Node*
Util::buildHuffmanTree(std::vector<std::pair<wchar_t, int>> frequences) {
    Heap::Node *left;
    Heap::Node *right;
    Heap::Node *top;
    globalCounter += 3;

    Heap* heap = new Heap(frequences, static_cast<int>(frequences.size()));
    globalCounter += 3;

    while (heap->currentSize > 1)
    {
        globalCounter += 2;

        left = heap->extractMin();
        right = heap->extractMin();
        globalCounter += 4;

        top = new Heap::Node(0, left->frequency + right->frequency, left, right);
        heap->insert(top);
        globalCounter += 6;
    }

    globalCounter += 1;
    return heap->extractMin();
};

Heap::Node*
Util::
buildShannonTree(std::vector<std::pair<wchar_t, int>> frequences)
{
    std::sort(frequences.begin(), frequences.end(), [](auto pairOne, auto pairTwo) {
        return pairOne.second < pairTwo.second;
    });
    globalCounter += 4 + 3 * (static_cast<int>(frequences.begin() - frequences.end()));

    return divideAndReturnParent(frequences, true);
}

Heap::Node*
Util::
divideAndReturnParent(std::vector<std::pair<wchar_t, int>> frequences, bool root)
{
    int size = static_cast<int>(frequences.size());
    globalCounter += 3;

    if (size > 1) {
        globalCounter += 1;

        int previousDifference = INT_MAX;
        int difference = 0;
        int separator = 0;
        globalCounter += 3;

        while( separator < size && difference < previousDifference) {
            separator++;
            globalCounter += 4;

            int first = 0;
            for (int f = 0; f < separator; ++f) first += frequences.at(f).second;
            globalCounter += 2 + 5 * (separator);

            int second = 0;
            for (int s = separator; s < size; ++s) second += frequences.at(s).second;
            globalCounter += 2 + 5 * (separator);

            if (separator > 1) previousDifference = difference;
            globalCounter += 2;

            difference = second - first;
            if (difference < 0) difference *= -1;
            globalCounter += 3;
        }
        separator--;
        globalCounter += 1;

        std::vector<std::pair<wchar_t,int>> leftSubarray(frequences.begin(), frequences.begin() + separator);
        std::vector<std::pair<wchar_t, int>> rightSubarray(frequences.begin() + separator, frequences.end());
        globalCounter += 8;
        
        Heap::Node* leftNode = divideAndReturnParent(leftSubarray, false);
        Heap::Node* rightNode = divideAndReturnParent(rightSubarray, false);
        globalCounter += 7;
        
        return new Heap::Node(0, leftNode->getFrequency() + rightNode->getFrequency(), leftNode, rightNode);
    } else if (size == 1) {
        Heap::Node* node = new Heap::Node(frequences[0].first, frequences[0].second, nullptr, nullptr);
        globalCounter += 7;
        
        if (root) {
            globalCounter += 4;
            return new Heap::Node(0, frequences[0].second, node, nullptr);
        } else {
            globalCounter += 2;
            return node;
        }
    }
}


// decoding functions

int
Util::
decodeFileLength(std::ifstream* stream)
{
    int length = 0;
    stream->read((char*)&length, sizeof(length));
    globalCounter += 6;

    return length;
}

Heap::Node*
Util::
decodeNode(BitReader* bitReader)
{
    if (bitReader->readBit() == 1) {
        globalCounter += 2;
        return new Heap::Node(bitReader->readWChar(), nullptr, nullptr);
    } else {
        globalCounter += 4;
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
        globalCounter += 3;
        
        if (node.value != 0) {
            stream.write((wchar_t*)&node.value, 1);
            node = *root;
            position++;
            globalCounter += 7;
        } else if (bitReader->readBit()) {
            node = *node.right;
            globalCounter += 6;
        } else {
            node = *node.left;
            globalCounter += 7;
        }
    }

}
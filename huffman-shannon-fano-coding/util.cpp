#ifndef UTIL_CPP
#define UTIL_CPP

#include "util.h"
#include "heap.h"
#include "bit_writer.h"

#include <fstream>
#include <codecvt>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <locale>
#include <bitset>



void
Util::
encodeNode(Heap::Node* node, BitWriter* writer) {
    if (node->isLeaf()) {
        writer->writeBit(1);
        writer->writeWChar(node->value);
    } else {
        writer->writeBit(0);
        encodeNode(node->left, writer);
        encodeNode(node->right, writer);
    }
}

void 
Util::
encodeFile(std::map<wchar_t, std::vector<bool>*>* huffmanCodes, char* inputFileName, char* outputFileName, BitWriter* writer)
{
    // initialize locale and stream
    std::locale locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wifstream stream(inputFileName);
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
}

void
Util::
fillHuffmanCodes(Heap::Node* root, std::map<wchar_t, std::vector<bool>*>* huffmanCodes)
{
    const int top = 0;
    const int maxTreeHeight = 50;
    int *buffer = new int[maxTreeHeight];
    Util::handleNode(root, buffer, top, huffmanCodes);
}

void
Util::
handleNode(Heap::Node* root, int* arr, int top, std::map<wchar_t, std::vector<bool>*>* huffmanCodes)
{   
    if (root->left) {
        arr[top] = 0;
        handleNode(root->left, arr, top + 1, huffmanCodes);
    }

    if (root->right) {
        arr[top] = 1;
        handleNode(root->right, arr, top + 1, huffmanCodes);
    }

    if (root->isLeaf()) {
        (*huffmanCodes)[root->value] = new std::vector<bool>(arr, arr + top);
    }
}

std::string
Util::
arrToString(int arr[], int n)
{
    std::string str = "";
    for (int i = 0; i < n; ++i) {
        char c = arr[i] + '0';
        str += c;
    }
    return str;
}

std::map<wchar_t, int>
Util::
getFileCharFrequencies(char* filename)
{
    // initialize locale and stream
    std::locale locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wifstream stream(filename);
    stream.imbue(locale);

    // create the container for ccounting 
    std::map<wchar_t, int> frequencies;

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

std::vector<std::pair<wchar_t, int>>
Util::
getVector(std::map<wchar_t, int> map)
{
    auto vector = std::vector<std::pair<wchar_t, int>>();

    for (std::pair<wchar_t, int> pair : map) {
        vector.push_back(pair);
    }

    return vector;
}

Heap::Node*
Util::buildHuffmanTree(std::vector<std::pair<wchar_t, int>> frequences) {
    Heap::Node *left;
    Heap::Node *right;
    Heap::Node *top;

    // Step 1: Create a min heap of capacity equal to size.  Initially, there are
    // modes equal to size.
    Heap* heap = new Heap(frequences, static_cast<int>(frequences.size()));

    // Iterate while size of heap doesn't become 1
    while (heap->currentSize > 1)
    {
        // Step 2: Extract the two minimum freq items from min heap
        left = heap->extractMin();
        right = heap->extractMin();

        // Step 3:  Create a new internal node with frequency equal to the
        // sum of the two nodes frequencies. Make the two extracted node as
        // left and right children of this new node. Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = new Heap::Node(NULL, left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        heap->insert(top);
    }

    // Step 4: The remaining node is the root node and the tree is complete.
    return heap->extractMin();
};

#endif
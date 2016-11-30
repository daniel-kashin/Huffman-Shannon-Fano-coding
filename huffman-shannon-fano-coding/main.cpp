//KDZ-1; Algorithms and data structures
//Kashin Danil Sergeevich, 154-2, 28-11-2016
//Visual Studio Community 2015 

#include "main.h"
#include "util.h"

#include <fstream>
#include <codecvt>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <bitset>



int main()
{
    encodeWithHuffman();
}

void
encodeWithHuffman()
{   
    std::map<wchar_t, int> frequenciesMap = Util::getFileCharFrequencies("test.txt");

    std::vector<std::pair<wchar_t, int>> frequencesVect =
        Util::getVector(frequenciesMap);

    BitWriter* writer = new BitWriter("output.txt");

    // build tree based on frequences of each letter
    Heap::Node* root = Util::buildHuffmanTree(frequencesVect);

    // get huffman codes from the tree
    auto huffmanCodes = new std::map<wchar_t, std::vector<bool>*>();
    Util::fillHuffmanCodes(root, huffmanCodes);

    // write tree to the header of the output file 
    Util::encodeNode(root, writer);

    // encode input file to the output
    Util::encodeFile(huffmanCodes, "test.txt", "output.txt", writer);
}

void 
decodeWithHuffman()
{
        
}

void
encodeWithShannonFano()
{

}

void
decodeWithShannonFano()
{

}


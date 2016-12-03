//KDZ-1; Algorithms and data structures
//Kashin Danil Sergeevich, 154-2, 28-11-2016
//Visual Studio Community 2015 

#include "main.h"
#include "util.h"

#include <fstream>
#include <codecvt>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <bitset>


// variable to count elementary operations
long long globalCounter;


int main(int argc, char* argv[])
{
    char arg = ' ';

    if (argc <= 1) { // no paramether: force read
        std::cout << "Paramether expected: 0 -- Shannon-Fano, default -- Huffman.\n";
        std::cin >> arg;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    } else { // get second console paramether
        arg = *argv[1];
    }

    if (arg == '0') {
        encodeWithShannonFano();
        decodeWithShannonFano();
    } else {
        encodeWithHuffman();
        decodeWithHuffman();
    }

    system("pause");
}


void
encodeWithHuffman()
{
    encode(true); // isHuffman = true
}

void
decodeWithHuffman()
{
    decode(true);
}

void
encodeWithShannonFano()
{
    encode(false);
}

void
decodeWithShannonFano()
{
    decode(false);
}

void
encode(bool isHuffman)
{
    std::unordered_map<wchar_t, int> frequenciesMap;
    std::string inputFileName;
    globalCounter = 0;

    // greeting
    std::string codingName = isHuffman ? "Huffman" : "Shannon-Fano";
    std::string message = "\nWrite filename to encode with " + codingName + " (.txt)";
    std::cout << message << std::endl;
    
    // read input filename
    bool fileDoesNotExist = true;
    while (fileDoesNotExist) {
        inputFileName = Util::readFileNameFromConsole(".txt");

        // get frequencies of all the letters in the input file
        frequenciesMap = Util::getFileCharFrequencies(inputFileName);

        fileDoesNotExist = (static_cast<int>(frequenciesMap.size()) == 0);

        if (fileDoesNotExist) std::cout << "Input file is either empty or does not exist." << std::endl;
    };

    // get output filename
    std::string* outputFileName = new std::string("");
    std::string fileExtension = isHuffman ? ".huff" : ".shan";
    Util::changeFileNameFormat(inputFileName, fileExtension, outputFileName);

    // build tree based on frequences of each letter
    std::vector<std::pair<wchar_t, int>> frequenciesVect = Util::getVector(frequenciesMap);
    Heap::Node* root;
    if (isHuffman) {
        root = Util::buildHuffmanTree(frequenciesVect);
    }
    else {
        root = Util::buildShannonTree(frequenciesVect);
    }

    // get unique code for each letter 
    auto huffmanCodes = new std::unordered_map<wchar_t, std::vector<bool>*>();
    Util::fillHuffmanCodes(root, huffmanCodes);

    // write length in letters of the input file
    std::ofstream* stream = new std::ofstream(*outputFileName, std::ios::binary);
    stream->write((char *)&root->frequency, sizeof(root->frequency));

    // write tree and file itself
    BitWriter* writer = new BitWriter(*stream);
    Util::encodeNode(root, writer);
    Util::encodeFile(huffmanCodes, inputFileName, writer);

    stream->close();
}

void
decode(bool isHuffman)
{
    std::string inputFileName;
    std::ifstream* stream = nullptr;
    globalCounter = 0;

    // greeting
    std::cout << "\n";
    std::string fileExtension = isHuffman ? ".huff" : ".shan";
    std::string message = "Write filename to decode with Huffman. (" + fileExtension + ")";
    std::cout << message << std::endl;
    
    // read input filename
    bool fileDoesNotExist = true;
    while (fileDoesNotExist) {
        inputFileName = Util::readFileNameFromConsole(fileExtension);
        stream = new std::ifstream(inputFileName, std::ios::binary);

        fileDoesNotExist = !stream->is_open();
        if (fileDoesNotExist) std::cout << "Input file is either empty or does not exist." << std::endl;
    }

    // get output filename
    std::string* outputFileName = new std::string("");
    std::string addition = isHuffman ? "-unz-h.txt" : "-unz-s.txt";
    Util::changeFileNameFormat(inputFileName, addition, outputFileName);

    // decode file
    try {
        int fileLength = Util::decodeFileLength(stream);

        BitReader* bitReader = new BitReader(*stream);

        Heap::Node* root = Util::decodeNode(bitReader);
        
        Util::decodeFile(bitReader, *outputFileName, root, fileLength);
    } catch (...) {
        std::cout << "Wrong file data. Please, don`t change .huff and .shan files.";
    }
}
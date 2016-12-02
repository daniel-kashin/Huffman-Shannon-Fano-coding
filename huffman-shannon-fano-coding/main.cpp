//KDZ-1; Algorithms and data structures
//Kashin Danil Sergeevich, 154-2, 28-11-2016
//Visual Studio Community 2015 

#include "main.h"
#include "util.h"
#include "bit_reader.h"
#include "bit_writer.h"

#include <fstream>
#include <codecvt>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <bitset>


int main()
{
    encodeWithHuffman();
    decodeWithHuffman();
}

void
encodeWithHuffman()
{
    std::unordered_map<wchar_t, int> frequenciesMap;
    std::string inputFileName;

    bool fileDoesNotExist = true;
    while (fileDoesNotExist)
    {
        inputFileName = Util::readFileNameFromConsole("Please, write filename to encode with Huffman. (.txt)", ".txt");

        // get frequencies of all the letters in the input file
        frequenciesMap = Util::getFileCharFrequencies(inputFileName);

        fileDoesNotExist = (static_cast<int>(frequenciesMap.size()) == 0);
        
        if (fileDoesNotExist) std::cout << "Input file is either empty or does not exist.\n" << std::endl;
    };


    std::string* outputFileName = new std::string("");
    Util::changeFileNameFormat(inputFileName, ".huff", outputFileName);

    std::vector<std::pair<wchar_t, int>> frequenciesVect =
        Util::getVector(frequenciesMap);

    // build tree based on frequences of each letter
    Heap::Node* root = Util::buildHuffmanTree(frequenciesVect);

    auto huffmanCodes = new std::unordered_map<wchar_t, std::vector<bool>*>();
    Util::fillHuffmanCodes(root, huffmanCodes);

    std::ofstream* stream = new std::ofstream(*outputFileName, std::ios::binary);

    // write length in letters of the input file
    stream->write((char *)&root->frequency, sizeof(root->frequency));

    BitWriter* writer = new BitWriter(*stream);

    // write tree to the header of the output file 
    Util::encodeNode(root, writer);

    // encode input file to the output
    Util::encodeFile(huffmanCodes, inputFileName, writer);

    stream->close();
}

void
decodeWithHuffman()
{
    std::string inputFileName;
    std::ifstream* stream = nullptr;

    bool fileDoesNotExist = true;
    while (fileDoesNotExist) {
        inputFileName = Util::readFileNameFromConsole("Please, write filename to decode with Huffman. (.huff)", ".huff");
        stream = new std::ifstream(inputFileName, std::ios::binary);

        fileDoesNotExist = !stream->is_open();
        if (fileDoesNotExist) std::cout << "Input file is either empty or does not exist.\n" << std::endl;
    }

    std::string* outputFileName = new std::string("");
    Util::changeFileNameFormat(inputFileName, "-unz-h.txt", outputFileName);
    
    int fileLength = Util::decodeFileLength(stream);

    BitReader* bitReader = new BitReader(*stream);

    Heap::Node* root = Util::decodeNode(bitReader);

    Util::decodeFile(bitReader, *outputFileName, root, fileLength);
}

void
encodeWithShannonFano(const char* inputFileName)
{
    std::string* newName = new std::string("");
    Util::changeFileNameFormat(inputFileName, ".shan", newName);
    const char* outputFileName = newName->c_str();


}

void
decodeWithShannonFano(const char* inputFileName)
{
    std::string* newName = new std::string("");
    Util::changeFileNameFormat(inputFileName, "-unz-h.txt", newName);
    const char* outputFileName = newName->c_str();


}


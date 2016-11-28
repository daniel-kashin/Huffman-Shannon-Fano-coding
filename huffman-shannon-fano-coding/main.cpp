//KDZ-1; Algorithms and data structures
//Kashin Danil Sergeevich, 154-2, 28-11-2016
//Visual Studio Community 2015 

#include "main_header.h"

#include <fstream>
#include <codecvt>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>

int main()
{
    codeWithHoffman();
}

void
codeWithHoffman()
{
    std::map<wchar_t, int> frequenciesMap = getFrequencies("test.txt");

    std::vector<std::pair<wchar_t, int>> frequencesVect = getSortedVector(frequenciesMap);
    


    system("pause");
}

void 
decodeWithHoffman()
{

}

void
codeWithShannonFano()
{

}

void
decodeWithShannonFano()
{

}

std::map<wchar_t, int>
getFrequencies(char* filename)
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
getSortedVector(std::map<wchar_t, int> map)
{
    auto vector = std::vector<std::pair<wchar_t, int>>();

    for (std::pair<wchar_t, int> kv : map) {
        vector.push_back(kv);
    }

    std::sort(vector.begin(), vector.end(), [](auto &left, auto &right) {
        return left.second > right.second;
    });

    return vector;
}
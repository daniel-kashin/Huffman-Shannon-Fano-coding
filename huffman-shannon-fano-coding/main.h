#include <map>
#include <vector>

std::map<wchar_t, int>
getFileCharFrequencies(char* filename);

std::vector<std::pair<wchar_t, int>>
getSortedVector(std::map<wchar_t, int> map);

void
codeWithHoffman();

void
decodeWithHoffman();

void
codeWithShannonFano();

void
decodeWithShannonFano();


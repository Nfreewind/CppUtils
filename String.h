#ifndef STRING_H
#define STRING_H
#include <iostream>
using namespace std;
class String
{
public:
	std::vector<string> split(string str, string separator);
	WCHAR *mbcsToUnicode(const char *zFilename);
	char *unicodeToUtf8(const WCHAR *zWideFilename);
	std::string encode(const std::string to_encode);
	std::string decode(const std::string to_decode);
};
#endif
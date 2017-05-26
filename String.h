#ifndef STRING_H
#define STRING_H
#include <iostream>
#include <string>
using namespace std;
class String
{
public:
	std::vector<string> split(string str, string separator);
	WCHAR *mbcsToUnicode(const char *zFilename);
	char *unicodeToUtf8(const WCHAR *zWideFilename);
	std::string encode(const std::string to_encode);
	std::string decode(const std::string to_decode);
	void trimleft(string &str,char c=' ');
	void trimright(string &str,char c=' ');
	void trim(string &str);
	int DecodeRemotFileCurl(const std::string & rStrRemotUrl, cv::Mat & decodeMat);
	int Write2String(char *data, size_t size, size_t nmemb, string * writerData);
};
#endif
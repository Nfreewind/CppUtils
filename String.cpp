#include "String.h"

std::vector<string> String::split(string str, string separator)
{
	std::vector<string> result;
	int cutAt;
	while ((cutAt = str.find_first_of(separator)) != str.npos)
	{
		if (cutAt > 0)
		{
			result.push_back(str.substr(0, cutAt));
		}
		str = str.substr(cutAt + 1);
	}
	if (str.length() > 0)
	{
		result.push_back(str);
	}
	return result;
}
WCHAR *String::mbcsToUnicode(const char *zFilename)
{
	int nByte;
	WCHAR *zMbcsFilename;
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, NULL, 0)*sizeof(WCHAR);
	zMbcsFilename = (WCHAR *)malloc(nByte*sizeof(zMbcsFilename[0]));
	if (zMbcsFilename == 0)
	{
		return 0;
	}
	nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, zMbcsFilename, nByte);
	if (nByte == 0)
	{
		free(zMbcsFilename);
		zMbcsFilename = 0;
	}
	return zMbcsFilename;
}

char *String::unicodeToUtf8(const WCHAR *zWideFilename)
{
	int nByte;
	char *zFilename;
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, 0, 0, 0, 0);
	zFilename = (char *)malloc(nByte);
	if (zFilename == 0)
		return 0;
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, zFilename, nByte, 0, 0);
	if (nByte == 0)
	{
		free(zFilename);
		zFilename = 0;
	}
	return zFilename;
}

std::string String::encode(const std::string to_encode)
{
	std::string res = to_encode;
	std::string key = "4962873";
	for (int i = 0, j = 0; res[j]; j++, i = (i + 1) % 7){
		res[j] += key[i];
		if (res[j] > 122) res[j] -= 90;
	}
	return res;
}
std::string String::decode(const std::string to_decode)
{
	std::string key = "4962873";
	std::string res = to_decode;
	for (int i = 0, j = 0; res[j]; j++, i = (i + 1) % 7){
		res[j] -= key[i];
		if (res[j] < 32) res[j] += 90;
	}
	return res;
}
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

void String::trimleft(string &str,char c/*=' '*/)
{
	//trim head
    size_t len = str.length();

	int i = 0;
	while(str[i] == c && str[i] != '\0'){
		i++;
	}
	if(i != 0){
		str = string(str,i,len-i);
	}
}

void String::trimright(string &str,char c/*=' '*/)
{
	//trim tail
	int i = 0;
    int len = (int)str.length();
		
	
	for(i = len - 1; i >= 0; --i ){
		if(str[i] != c){
			break;
		}
	}

	str = string(str,0,i+1);
}

void String::trim(string &str)
{
	//trim head
    int len = (int)str.length();

	int i   = 0;

	while(isspace(str[i]) && str[i] != '\0'){
		i++;
	}
	if(i != 0){
		str = string(str,i,len-i);
	}

	//trim tail
	len = (int)str.length();

	for(i = len - 1; i >= 0; --i ){
		if(!isspace(str[i])){
			break;
		}
	}
	str = string(str,0,i+1);
}

int String::DecodeRemotFileCurl(const std::string & rStrRemotUrl, cv::Mat & decodeMat)
{
	std::string strRevData;

	CURL * pCurlHand = curl_easy_init();

	if (pCurlHand == NULL)
	{
		return -1;
	}
	curl_easy_setopt(pCurlHand, CURLOPT_URL, rStrRemotUrl.c_str());
	curl_easy_setopt(pCurlHand, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(pCurlHand, CURLOPT_WRITEDATA, &strRevData);
	curl_easy_setopt(pCurlHand, CURLOPT_WRITEFUNCTION, Write2String);
	curl_easy_setopt(pCurlHand, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(pCurlHand, CURLOPT_CONNECTTIMEOUT, 4);
	curl_easy_setopt(pCurlHand, CURLOPT_TIMEOUT, 30);

	CURLcode dwCurlRet = curl_easy_perform(pCurlHand);
	if (dwCurlRet != CURLE_OK)
	{
		curl_easy_cleanup(pCurlHand);
		return -1;
	}

	cv::Mat Bitstream(cv::Size((int)strRevData.size(), 1),
		CV_8U,
		(unsigned char *)strRevData.c_str());

	decodeMat = cv::imdecode(Bitstream, CV_LOAD_IMAGE_COLOR);
	curl_easy_cleanup(pCurlHand);
	return 0;
}

int String::Write2String(char *data, size_t size, size_t nmemb, string * writerData)
{
	unsigned long sizes = size * nmemb;

	if (writerData == NULL)
	{
		return 0;
	}

	writerData->append(data, sizes);

	return sizes;
}
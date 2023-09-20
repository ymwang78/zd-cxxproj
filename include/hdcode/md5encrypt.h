#pragma once

class CMD5Encrypt
{
private:
	CMD5Encrypt() {}
public:
	static void EncryptData(char * pszSrcData, size_t nSrcLen,  char szMD5Result[], char szMD5Res[]);
};

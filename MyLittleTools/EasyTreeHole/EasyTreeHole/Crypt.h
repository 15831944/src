#pragma once

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <iostream>
using namespace std;
#include <stdlib.h>
#include "..//include//aes.h"

#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "..\\lib\\x86\\Debug\\cryptlib.lib")
#else
#pragma comment(lib, "..\\lib\\x86\\Debug\\cryptlib.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "..\\lib\\x86\\Release\\cryptlib.lib")
#else
#pragma comment(lib, "..\\lib\\x86\\Release\\cryptlib.lib")
#endif
#endif

using namespace CryptoPP;

#include <windows.h>

int AESCrypto(unsigned char *aesKey, byte *byteInData, int iInSize, byte **byteOut);

int AESDeCrypto(unsigned char *aesKey, byte *byteInData, int iInSize, byte **byteOut);
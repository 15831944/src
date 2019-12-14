
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <iostream>
using namespace std;
#include <stdlib.h>
#include "..//include//aes.h"
#include <windows.h>
#include "stdafx.h"
#include "Crypt.h"

int AESCrypto(unsigned char *aesKey, byte *byteInData, int iInSize, byte **byteOut) {
	int iRet = 0;

	int iBlocks = iInSize / AES::BLOCKSIZE;
	if (iInSize % AES::BLOCKSIZE != 0)
		iBlocks++;
	int iOutDataSize = iBlocks * AES::BLOCKSIZE;
	*byteOut = new byte[iOutDataSize];
	if (*byteOut == NULL)
		return 0;

	AESEncryption aesEncryptor; //������     
	aesEncryptor.SetKey(aesKey, AES::DEFAULT_KEYLENGTH); //�趨������Կ

	unsigned char xorBlock[AES::BLOCKSIZE]; //�����趨Ϊȫ��
	memset(xorBlock, 0, AES::BLOCKSIZE); //����
	unsigned char inBlock[AES::BLOCKSIZE]; //Ҫ���ܵ����ݿ�
	unsigned char outBlock[AES::BLOCKSIZE]; //���ܺ�����Ŀ�

	int pos = 0;
	do
	{
		int relaysize = iInSize - pos;
		int cpsize = relaysize > AES::BLOCKSIZE ? AES::BLOCKSIZE : relaysize;
		memset(inBlock, 0, AES::BLOCKSIZE);
		memset(outBlock, 0, AES::BLOCKSIZE);
		memcpy(inBlock, byteInData + pos, cpsize);
		aesEncryptor.ProcessAndXorBlock(inBlock, xorBlock, outBlock); //����
		memcpy(*byteOut + iRet, outBlock, AES::BLOCKSIZE);
		iRet += AES::BLOCKSIZE;
		pos += cpsize;

	} while (pos < iInSize - 1);
	return iRet;
}

int AESDeCrypto(unsigned char *aesKey, byte *byteInData, int iInSize, byte **byteOut) {
	int iRet = 0;

	int iBlocks = iInSize / AES::BLOCKSIZE;
	if (iInSize % AES::BLOCKSIZE != 0)
		iBlocks++;
	int iOutDataSize = iBlocks * AES::BLOCKSIZE;
	*byteOut = new byte[iOutDataSize + 2];
	if (*byteOut == NULL)
		return 0;

	RtlZeroMemory(*byteOut, iOutDataSize + 2);

	AESDecryption aesDecryptor;
	aesDecryptor.SetKey(aesKey, AES::DEFAULT_KEYLENGTH);

	unsigned char xorBlock[AES::BLOCKSIZE]; //�����趨Ϊȫ��
	memset(xorBlock, 0, AES::BLOCKSIZE); //����
	unsigned char inBlock[AES::BLOCKSIZE]; //Ҫ���ܵ����ݿ�
	unsigned char outBlock[AES::BLOCKSIZE]; //���ܺ�����Ŀ�

	int pos = 0;
	do
	{
		int relaysize = iInSize - pos;
		int cpsize = relaysize > AES::BLOCKSIZE ? AES::BLOCKSIZE : relaysize;
		memset(inBlock, 0, AES::BLOCKSIZE);
		memset(outBlock, 0, AES::BLOCKSIZE);
		memcpy(inBlock, byteInData + pos, cpsize);
		aesDecryptor.ProcessAndXorBlock(inBlock, xorBlock, outBlock);
		memcpy(*byteOut + iRet, outBlock, AES::BLOCKSIZE);
		iRet += AES::BLOCKSIZE;
		pos += cpsize;

	} while (pos < iInSize);
	return iRet;
}
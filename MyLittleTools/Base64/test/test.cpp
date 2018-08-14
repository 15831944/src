#include "stdafx.h"
#include <windows.h>


//输出错误信息
LPTSTR ThrowMes(LPTSTR mes, int err) {
	memset(mes, 0, _tcslen(mes) * sizeof(TCHAR));
	//int err = GetLastError();
	//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), mes, 1024, NULL);
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 1033, mes, 1024, NULL);//1033是英语的language ID

	if(_tcslen(mes) == 0)
		_tcscpy(mes, TEXT("[double err : no such error code]\n"));

	return mes;
}


TCHAR base64Char[65] = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");


LPTSTR base64_encode(LPTSTR input, LPTSTR out){

	int iLen = _tcslen(input);
	int len = iLen / 3 * 3;
	
	int i = 0, j = 0;
	while(i < len){
		out[j] = base64Char[input[i]>>2];
		out[j+1] = base64Char[((input[i]&0x3)<<4)|(input[i+1]>>4)];
		out[j+2] = base64Char[((input[i+1]&0xf)<<2)|(input[i+2]>>6)];
		out[j+3] = base64Char[(input[i+2]&0x3f)];

		i += 3;
		j += 4;
	}

	switch(iLen % 3){
	case 2:
		out[j] = base64Char[input[i] >> 2];
		out[j+1] = base64Char[((input[i]&0x3)<<4)|(input[i+1]>>4)];
		out[j+2] = base64Char[((input[i+1]&0xf)<<2)];
		out[j+3] = TEXT('=');
		break;
	case 1:
		out[j] = base64Char[input[i] >> 2];
		out[j + 1] = base64Char[((input[i] & 0x3) << 4)];
		out[j + 2] = TEXT('=');
		out[j + 3] = TEXT('=');
		break;
	}

	return out;
}


int Base2Char(TCHAR a){
	if(a >= TEXT('A') && a <= TEXT('Z'))
		return a - TEXT('A');
	if(a >= TEXT('a') && a <= TEXT('z'))
		return a - TEXT('a') + 26;
	if (a >= TEXT('0') && a <= TEXT('9'))
		return a - TEXT('0') + 52;
	if(a == TEXT('+'))
		return 63;
	return 63;
}


LPTSTR base64_decode(LPTSTR input, LPTSTR out){
	
	int iLen = _tcslen(input);

	int i = 0, j = 0;

	while(i < iLen - 4){
		out[j] = Base2Char(input[i])<<2 | Base2Char(input[i+1]) >> 4;
		out[j+1] = (Base2Char(input[i+1])&0xf)<<4 | Base2Char(input[i+2]) >> 2;
		out[j+2] = (Base2Char(input[i+2])&0x3)<<6 | Base2Char(input[i+3]);

		i += 4;
		j += 3;
	}

	if(input[iLen - 2] == TEXT('=')){
		out[j] = Base2Char(input[i]) << 2 | Base2Char(input[i + 1]) >> 4;
		out[j+1] = 0;
	}
	else if (input[iLen - 1] == TEXT('=')) {
		out[j] = Base2Char(input[i]) << 2 | Base2Char(input[i + 1]) >> 4;
		out[j+1] = (Base2Char(input[i+1])&0xf)<<4 | Base2Char(input[i+2]) >> 2;
		out[j+2] = 0;
	}
	else{
		out[j] = Base2Char(input[i])<<2 | Base2Char(input[i+1]) >> 4;
		out[j+1] = (Base2Char(input[i+1])&0xf)<<4 | Base2Char(input[i+2]) >> 2;
		out[j+2] = (Base2Char(input[i+2])&0x3)<<6 | Base2Char(input[i+3]);
		out[j+3] = 0;
	}

	return out;
}


int _tmain(){

	TCHAR data[1024] = TEXT("");
	TCHAR out[1364] = { 0 };
	int mode = 0;
	
	_tprintf(TEXT("input the data(raw<1024 BYTES;code<1364 BYTES. ENCODE-> 0, DECODE-> 1) :\n"));

	while(EOF != _tscanf_s(TEXT("%s"), data, 1024)){
		_tscanf_s(TEXT("%d"), &mode);
		if(mode == 0){
			_tprintf(TEXT("Encoded code :\n%s\n"), base64_encode(data, out));
		}
		else if(mode == 1){
			_tprintf(TEXT("Decoded code :\n%s\n"), base64_decode(data, out));
		}
		_tprintf(TEXT("\ninput the data(raw<1024 BYTES;code<1364 BYTES) :\n"));
	}

	return 0;
}
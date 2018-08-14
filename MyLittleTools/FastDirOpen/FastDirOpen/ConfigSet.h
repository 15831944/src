#pragma once
#include "stdafx.h"

//BOOL WriteItemToFile(HANDLE hWrite, LPTSTR lpName, LPTSTR lpPath);
BOOL WriteItemToFile(HANDLE hWrite, LPTSTR lpName, LPTSTR lpPath, LPTSTR lpRemark);

BOOL ReadFromFile(LPTSTR lpFilePath, __out LPTSTR lpRetData);

BOOL GetItemFromData(LPTSTR lpReadData, LPTSTR lpName, LPTSTR lpPath, LPTSTR lpRemark);

//BOOL GetItemFromData(__in __out LPTSTR lpReadData, __out LPTSTR lpName, __out LPTSTR lpPath);
//BOOL DeleteAnItem(LPTSTR lpIniPath, LPTSTR lpName, LPTSTR lpPath);

BOOL DeleteAnItem(LPTSTR lpIniPath, LPTSTR lpName, LPTSTR lpPath, LPTSTR lpRemark);
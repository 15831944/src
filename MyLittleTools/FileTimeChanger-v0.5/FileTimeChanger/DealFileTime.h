#pragma once
#include "stdafx.h"
BOOL GetFileTimes(LPTSTR lpFilePath, FILE_TIMES* pft);
BOOL ChangeFileTimes(LPTSTR lpFilePath, FILE_TIMES* pft);
FILETIME SystemTime2Filetime(SYSTEMTIME ct);
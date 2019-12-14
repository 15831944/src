#include "stdafx.h"
#include "CMyEdit.h"

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

CMyEdit::CMyEdit()
{
	mSelectAll = FALSE;
	mHeight = 22;
}

CMyEdit::~CMyEdit()
{
}

void CMyEdit::OnDropFiles(HDROP hDropInfo)
{
	if (hDropInfo)//拖拽消息指针
	{
		TCHAR szFilePath[PATH_LEN] = { 0 };
		int nDrag = DragQueryFile(hDropInfo, -1, NULL, 0);//获取一共拖拽的文件的个数

		for (int i = 0; i < nDrag; i++) {
			RtlZeroMemory(szFilePath, PATH_LEN);
			DragQueryFile(hDropInfo, i, szFilePath, 1024);//获得第一个文件的路径m_szFilePath，返回路径长度

			mFilePathEdit->SetWindowTextW(szFilePath);
		}
	}

	DragFinish(hDropInfo);
}

BOOL CMyEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (GetKeyState(VK_CONTROL) & 0x80))
	{
		switch (pMsg->wParam)
		{
		case 'A':
		case 'a':
			if (mSelectAll)
			{
				this->SetSel(mLastStart, mLastEnd);         // 取消全选
			}
			else
			{
				this->GetSel(mLastStart, mLastEnd);
				this->SetSel(0, -1);          // 全选
			}
			mSelectAll = !mSelectAll;
			break;

		case 'V':
		case 'v':
			mSelectAll = FALSE;
			this->Paste();
			break;

		case 'C':
		case 'c':
			this->Copy();
			break;
		case VK_ADD:
			mHeight+=5;
			this->SetTextFont(mHeight);
			break;
		case VK_SUBTRACT:
			if(mHeight > 5)
				mHeight-=5;
			this->SetTextFont(mHeight);
			break;
		default:
			break;
		}

		return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

bool CMyEdit::SetTextFont(int iHeight)
{
	CFont * f;
	f = new CFont;
	f->CreateFontW(iHeight, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FALSE, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("")); // lpszFac 
	this->SetFont(f);
	return true;
}
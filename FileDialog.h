#include <windows.h>
#pragma once
class FileDialog
{
public:
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

protected:
	virtual BOOL ActionType(OPENFILENAME ofn, HWND hEdit, LPCTSTR szFileName) = NULL;
	virtual BOOL Action(HWND hEdit, LPCTSTR pszFileName) = NULL;
	BOOL Dialog(HWND hwnd, HWND hEdit);
};


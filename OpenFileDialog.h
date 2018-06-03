#pragma once
#include "FileDialog.h"
class OpenFileDialog :
	public FileDialog
{
public:
	OpenFileDialog();
	BOOL OpenFile(HWND hwnd, HWND hEdit);
protected:
	BOOL Action(HWND hEdit, LPCTSTR pszFileName) override;
	BOOL ActionType(OPENFILENAME ofn, HWND hEdit, LPCTSTR szFileName) override;
	
};


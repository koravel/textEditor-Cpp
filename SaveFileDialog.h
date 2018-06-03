#pragma once
#include "FileDialog.h"
class SaveFileDialog :
	public FileDialog
{
public:
	SaveFileDialog();
	BOOL SaveFile(HWND hwnd, HWND hEdit);
	BOOL Action(HWND hEdit, LPCTSTR pszFileName) override;
protected:
	BOOL ActionType(OPENFILENAME ofn, HWND hEdit, LPCTSTR szFileName) override;
};


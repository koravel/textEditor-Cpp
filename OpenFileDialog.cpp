#include "OpenFileDialog.h"

OpenFileDialog::OpenFileDialog()
{
}

BOOL OpenFileDialog::OpenFile(HWND hwnd, HWND hEdit)
{
	return Dialog(hwnd, hEdit);
}

BOOL OpenFileDialog::ActionType(OPENFILENAME ofn, HWND hEdit, LPCTSTR szFileName)
{
	if (GetOpenFileName(&ofn))
	{
		if (Action(hEdit, szFileName)) return true;
		return false;
	}
	return false;
}

BOOL OpenFileDialog::Action(HWND hEdit, LPCTSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize;

		dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0xFFFFFFFF)
		{
			LPSTR pszFileText;

			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (pszFileText != NULL)
			{
				DWORD dwRead;

				if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{
					pszFileText[dwFileSize] = 0; // Add null terminator
					if (SetWindowText(hEdit, pszFileText))
						bSuccess = TRUE; // It worked!
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}




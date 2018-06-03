#include "SaveFileDialog.h"

SaveFileDialog::SaveFileDialog()
{
}

BOOL SaveFileDialog::SaveFile(HWND hwnd, HWND hEdit)
{
	return Dialog(hwnd ,hEdit);
}

BOOL SaveFileDialog::ActionType(OPENFILENAME ofn, HWND hEdit, LPCTSTR szFileName)
{
	if (GetSaveFileName(&ofn))
	{
		if (Action(hEdit, szFileName)) return true;
		return false;
	}
	return false;
}

BOOL SaveFileDialog::Action(HWND hEdit, LPCTSTR pszFileName)
{

	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength;

		dwTextLength = GetWindowTextLength(hEdit);
		// No need to bother if there's no text.
		if (dwTextLength > 0)
		{
			LPSTR pszText;
			DWORD dwBufferSize = dwTextLength + 1;

			pszText = (LPSTR)GlobalAlloc(GPTR, dwBufferSize);
			if (pszText != NULL)
			{
				if (GetWindowText(hEdit, pszText, dwBufferSize))
				{
					DWORD dwWritten;

					if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
						bSuccess = TRUE;
				}
				GlobalFree(pszText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}


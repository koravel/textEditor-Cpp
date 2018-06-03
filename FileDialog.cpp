#include "FileDialog.h"

BOOL FileDialog::Dialog(HWND hwnd, HWND hEdit)
{
	ZeroMemory(&ofn, sizeof(ofn)); // initialise it to 0 instead of NULL

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH; 
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//OFN_OVERWRITEPROMPT - generate a message box if the selected file already exists.
	ofn.lpstrDefExt = "txt";
	return ActionType(ofn, hEdit, szFileName);
}

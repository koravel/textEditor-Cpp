#include <windows.h>         // подключение библиотеки с функциями API
#include <stdio.h>
#include <ctype.h>
#include <string>
#include "resource.h"
#include "SaveFileDialog.h"
#include "OpenFileDialog.h"
#include "TextAnalytics.h"

using namespace std;

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = "TextEditor";
LPCTSTR szTitle = "Text Editor";

// Предварительное описание функций

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Основная программа 
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	// Регистрация класса окна 
	MyRegisterClass(hInstance);

	// Создание окна приложения
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  Регистрирует класс окна 

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;	// стиль окна
	wcex.lpfnWndProc = (WNDPROC)WndProc; // оконная процедура
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;		// указатель приложения
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// определение иконки
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);    // опре-деление курсора
	wcex.hbrBackground = GetSysColorBrush(0);   // установка фона
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);		// определение меню
	wcex.lpszClassName = szWindowClass;	// имя класса
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex); // регистрация класса окна
}

// FUNCTION: InitInstance(HANDLE, int)
// Создает окно приложения и сохраняет указатель приложения в переменной hInst

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // сохраняет указатель приложения в переменной hInst

	hWnd = CreateWindow(szWindowClass, // имя класса окна
		szTitle,   // имя приложения
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,// | WS_MAXIMIZEBOX | WS_SIZEBOX, // стиль окна
		0,	// положение по Х
		0, 	// положение по Y
		1280,    // размер по Х
		720,    // размер по Y
		NULL,	// описатель родительского окна
		NULL,       // описатель меню окна
		hInstance,  // указатель приложения
		NULL);     // параметры создания.

	if (!hWnd) // Если окно не создалось, функция возвращает FALSE
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_NORMAL);		// Показать окно
	UpdateWindow(hWnd);			// Обновить окно
	return TRUE;				//Успешное завершение функции
}

HWND TextBox;
HWND Counter;
SaveFileDialog sfDialog;
OpenFileDialog ofDialog;

char *currentFileName = "";

enum State
{
	_new = 0,
	_newEdited = 1,
	_notNew = 2,
	_notNewEdited = 3
};

int state = _new;

void Empty(HWND hwnd)
{
	SetWindowText(TextBox, "");
	SetWindowText(hwnd, szTitle);
	currentFileName = "";
}

BOOL SaveChangesMessage(HWND hwnd)
{
	return MessageBox(hwnd, "Сохранить изменения?", "Сохранение", MB_YESNOCANCEL);
}

BOOL SaveAsDialog(HWND hWnd)
{
	if (sfDialog.SaveFile(hWnd, TextBox))
	{
		SetWindowText(hWnd, sfDialog.szFileName);
		currentFileName = sfDialog.szFileName;
		return true;
	}
	return false;
}

BOOL Save(HWND hwnd)
{
	if (sfDialog.Action(TextBox, currentFileName))
	{
		SetWindowText(hwnd, currentFileName);
		return true;
	}
	return false;
}

BOOL New(HWND hwnd)
{
	return false;
}

BOOL OpenDialog(HWND hwnd)
{
	if (ofDialog.OpenFile(hwnd, TextBox))
	{
		SetWindowText(hwnd, ofDialog.szFileName);
		SetWindowText(Counter, SetTextInfo(TextBox).c_str());
		currentFileName = ofDialog.szFileName;
		return true;
	}
	return false;
}

char* append(char* charArray, char a)
{
	size_t length = strlen(charArray);

	char* result = new char[length + 2];

	strcpy_s(result, strlen(result), charArray);
	result[length] = a;
	result[length + 1] = '\0';

	return result;
}

void _newProcessing(HWND hwnd, int element)
{
	switch (element)
	{
		case ID_HEADER_OPEN:
			if (OpenDialog(hwnd)) state = _notNew;
		break;

		case ID_HEADER_SAVE:
		case ID_HEADER_SAVE_AS:
			if (SaveAsDialog(hwnd)) state = _notNew;
		break;

		case ID_HEADER_EXIT:
			ExitProcess(0);
		break;
	}
}

void _newEditedProcessing(HWND hwnd, int element)
{
	int messageResult;

	switch (element)
	{
		case ID_HEADER_NEW:
			messageResult = SaveChangesMessage(hwnd);

			if (messageResult == IDYES)
			{
				SaveAsDialog(hwnd);
				Empty(hwnd);
				state = _new;
			}
			else if (messageResult != IDCANCEL)
			{
				Empty(hwnd);
				state = _new;
			}
		break;

		case ID_HEADER_OPEN:
			messageResult = SaveChangesMessage(hwnd);

			if (messageResult == IDYES)
			{
				if (SaveAsDialog(hwnd)) state = _notNew;
			}
			else if (messageResult != IDCANCEL)
			{
				if (OpenDialog(hwnd)) state = _notNew;
			}
		break;

		case ID_HEADER_SAVE:
		case ID_HEADER_SAVE_AS:
			if (SaveAsDialog(hwnd)) state = _notNew;
		break;

		case ID_HEADER_EXIT:
			messageResult = SaveChangesMessage(hwnd);

			if (messageResult == IDYES) SaveAsDialog(hwnd);
			else if (messageResult != IDCANCEL)
			{
				ExitProcess(0);
			}
		break;
	}
}

void _notNewProcessing(HWND hwnd, int element)
{
	switch (element)
	{
	case ID_HEADER_NEW:
		Empty(hwnd);
		state = _new;
		break;

	case ID_HEADER_OPEN:
		OpenDialog(hwnd);
		break;

	case ID_HEADER_SAVE:
		Save(hwnd);//if file has been changed outside, it will be overwritten
		break;

	case ID_HEADER_SAVE_AS:
		SaveAsDialog(hwnd);
		break;

	case ID_HEADER_EXIT:
		ExitProcess(0);
		break;
	}
}

void _notNewEditedProcessing(HWND hwnd, int element)
{
	int messageResult;

	switch (element)
	{
	case ID_HEADER_NEW:
		messageResult = SaveChangesMessage(hwnd);

		if (messageResult == IDYES)
		{
			SaveAsDialog(hwnd);
			Empty(hwnd);
			state = _new;
		}
		else if (messageResult != IDCANCEL)
		{
			Empty(hwnd);
			state = _new;
		}
		break;

	case ID_HEADER_OPEN:
		messageResult = SaveChangesMessage(hwnd);

		if (messageResult == IDYES)
		{
			if (SaveAsDialog(hwnd)) state = _notNew;
		}
		if (messageResult != IDCANCEL)
		{
			if (OpenDialog(hwnd)) state = _notNew;
		}
		break;

	case ID_HEADER_SAVE:
		Save(hwnd);//if file has been changed outside, it will be overwritten
		break;

	case ID_HEADER_SAVE_AS:
		SaveAsDialog(hwnd);
		break;

	case ID_HEADER_EXIT:

		messageResult = SaveChangesMessage(hwnd);

		if (messageResult == IDYES) SaveAsDialog(hwnd);
		else if (messageResult != IDCANCEL)
		{
			ExitProcess(0);
		}
		break;
	}
}

void StatesProcessing(HWND hwnd, int element)
{
	switch (state)
	{
		case _new: _newProcessing(hwnd, element);
		break;

		case _newEdited: _newEditedProcessing(hwnd, element);
		break;

		case _notNew: _notNewProcessing(hwnd, element);
		break;

		case _notNewEdited: _notNewEditedProcessing(hwnd, element);
		break;
	}
}

HWND CreateEditControl(HWND hwnd, DWORD style, bool readonly, int x, int y, int width, int height)
{
	if (readonly) style = style | ES_READONLY;

	return CreateWindowEx(NULL, TEXT("Edit"), NULL,
		style,
		x, y, width, height,
		hwnd, NULL, NULL, NULL);
}

HWND CreateTextField(HWND hwnd, int x, int y, int width, int height)
{
	DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL;

	return CreateEditControl(hwnd, style, false, x, y, width, height);
}

HWND CreateTextBox(HWND hwnd, int x, int y, int width, int height)
{
	DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER;
	
	return CreateEditControl(hwnd, style, true, x, y, width, height);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int element = LOWORD(wParam);
	
	switch (message)
	{
		case WM_CREATE:
			TextBox = CreateTextField(hwnd, 0, 0, 1264, 641);
			Counter = CreateTextBox(hwnd, 0, 641, 1264, 20);			
		break;

		case WM_COMMAND: 
			StatesProcessing(hwnd, element);
		break;

		case WM_CLOSE:
			if (state == _newEdited || state == _notNewEdited)
				if (SaveChangesMessage(hwnd) == IDYES) SaveAsDialog(hwnd);
			DestroyWindow(hwnd);
		break;

		case WM_DESTROY: 
			PostQuitMessage(0);
		break;

		default: return DefWindowProc(hwnd, message, wParam, lParam); // Обработка сообщений, которые не обработаны пользователем
	}

	switch (HIWORD(wParam))
	{
		case EN_CHANGE:
			if (TextBox == (HWND)lParam)
			{
				SetWindowText(Counter, SetTextInfo(TextBox).c_str());

				if (state == _new) state = _newEdited;
				else if (state == _notNew) state = _notNewEdited;

				if (state == _notNewEdited) SetWindowText(hwnd, append(currentFileName, '*'));
				else if (state == _newEdited) SetWindowText(hwnd, append((char*)szTitle, '*'));
			}
		break;
	}
	return 0;
}

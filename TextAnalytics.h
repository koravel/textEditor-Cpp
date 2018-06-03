#pragma once
#include <windows.h>
#include <string>

using namespace std;

bool isCharSentenceSeparator(char symbol);

bool isRusChar(char symbol);

string SetTextInfo(HWND hwnd);

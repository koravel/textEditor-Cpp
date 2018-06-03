#include "TextAnalytics.h"

bool isCharSentenceSeparator(char symbol)
{
	char separators[] = { '.', '?', '!' };
	for (int i = 0; i < 3; i++)
	{
		if (symbol == separators[i]) return true;
	}
	return false;
}

bool isRusChar(char symbol)
{
	return ((symbol >= 'ј' && symbol <= '€') || symbol == 'Є' || symbol == '®');
}

bool isSymbol(char symbol)
{
	char separators[] = { ',', '@', '#', 'є', '$', ';', ':', '^', '&', '*', '(', ')', '`', '~', '{', '}', '[', ']', '\\', '|', '/', '%', '<', '>', '+', '=' };
	for (int i = 0; i < 26; i++)
	{
		if (symbol == separators[i]) return true;
	}
	
	if (iswdigit(symbol)) return true;

	return false;
}

bool isDirectOrationSymbol(char symbol)
{
	char separators[] = { ',', 'Ч' };
	for (int i = 0; i < 2; i++)
	{
		if (symbol == separators[i]) return true;
		else if (isCharSentenceSeparator(symbol)) return true;
	}
	return false;
}

bool isQuote(char symbol)
{
	char quotes[] = { '\'', '"', 'Ђ', 'ї', 'Д', 'У', 'Ф','С', 'Т' };
	for (int i = 0; i < 3; i++)
	{
		if (symbol == quotes[i]) return true;
	}
	return false;
}

string SetTextInfo(HWND hwnd)
{
	int length = GetWindowTextLength(hwnd);
	char* text = new char[length + 1];

	GetWindowText(hwnd, text, length + 1);

	int numberOfLetters = 0,
		  numberOfWords = 0,
		  numberOfSentences = 0,
		  numberOfRusLetters = 0;

	bool isRusLetter = false,
		isLetter = false,
		inQuotes = false;

	int i = 0;

	while (i < length)
	{
		isRusLetter = isRusChar(text[i]);
		isLetter = iswalpha(text[i]);

		if (isLetter || isRusLetter)
		{
			while ((isLetter || isRusLetter || isSymbol(text[i])) && i < length)
			{
				if(isLetter) numberOfLetters++;
				else if (isRusLetter) numberOfRusLetters++;
				i++;
				isRusLetter = isRusChar(text[i]);
				isLetter = iswalpha(text[i]);
			}
			i--;
			numberOfWords++;
		}
		else
		{
			isLetter = false;
			isRusLetter = false;
		}

		if (isQuote(text[i]))
		{
			if (inQuotes)
			{
				while (!isQuote(text[i]) && i < length)
				{
					if (isDirectOrationSymbol(text[i])) numberOfSentences++;
					i++;
				}
				i--;
				inQuotes = false;
			}
			else inQuotes = true;
		}
		else if (isCharSentenceSeparator(text[i]) && !isCharSentenceSeparator(text[i - 1])) numberOfSentences++;

		i++;
	}
	string language = "";
	if (numberOfRusLetters > numberOfLetters) language = "–усский";
	else language = "English";
	if (!numberOfRusLetters && !numberOfLetters || numberOfRusLetters == numberOfLetters) language = "Ќеизвестно";

	return	"Ѕукв: " + to_string(numberOfLetters + numberOfRusLetters) + 
			" | —лов: " + to_string(numberOfWords) + 
			" | ѕредложений: " + to_string(numberOfSentences) + 
			" | язык: " + language;
}
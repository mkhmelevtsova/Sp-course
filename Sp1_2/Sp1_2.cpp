#include "pch.h"
void AnsiUnicode(LPCTSTR szSourceFile, LPCTSTR szDestFile);
void UnicodeAnsi(LPCTSTR szSourceFile, LPCTSTR szDestFile);
LPWSTR ANSIToUnicode(char* src);
LPSTR UnicodeToANSI(LPCWSTR src);

int main(int argc, TCHAR* argv[])
{
	if (argc == 1)
	{
		printf("No arguments");
		return 0;
	}
	switch (*argv[1])
	{
	case 'a':
	{
		LPCTSTR _sourceFileName = (LPCTSTR)argv[2];
		LPCTSTR _destinationFileName = (LPCTSTR)argv[3];
		printf((const char*)_sourceFileName);
		printf((const char*)_destinationFileName);
		AnsiUnicode(_sourceFileName, _destinationFileName);
		break;
	}
	case 'u':
	{
		LPCTSTR _sourceFileName = (LPCTSTR)argv[2];
		LPCTSTR _destinationFileName = (LPCTSTR)argv[3];
		UnicodeAnsi(_sourceFileName, _destinationFileName);
		break;
	}
	default:
		printf("\nInvalid arguments!");
		break;
	}
	return 0;
}

void AnsiUnicode(LPCTSTR szSourceFile, LPCTSTR szDestFile)
{
	HANDLE hFile;
	if (!(hFile = CreateFile(szSourceFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		return;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwBytesRead;
	CHAR* szBuf = new CHAR[dwFileSize + 1]; szBuf[dwFileSize] = '\0';
	ReadFile(hFile, szBuf, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);

	if (!(hFile = CreateFile(szDestFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		return;
	}
	WCHAR* szBufW = ANSIToUnicode(szBuf);
	DWORD dwBytesToWrite = lstrlenW(szBufW) * sizeof(WCHAR);
	DWORD dwBytesWritten;
	WriteFile(hFile, szBufW, dwBytesToWrite, &dwBytesWritten, NULL);
	CloseHandle(hFile);
	delete[] szBuf;
}
void UnicodeAnsi(LPCTSTR szSourceFile, LPCTSTR szDestFile)
{
	HANDLE hFile;
	if (!(hFile = CreateFile(szSourceFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		return;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwBytesRead;
	WCHAR* szBufW = new WCHAR[dwFileSize / sizeof(WCHAR) + 1]; szBufW[dwFileSize / sizeof(WCHAR)] = 0;
	ReadFile(hFile, szBufW, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);

	if (!(hFile = CreateFile(szDestFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		return;
	}
	CHAR* szBuf = UnicodeToANSI(szBufW);
	DWORD dwBytesToWrite = strlen(szBuf);
	DWORD dwBytesWritten;
	WriteFile(hFile, szBuf, dwBytesToWrite, &dwBytesWritten, NULL);
	CloseHandle(hFile);
	delete[] szBuf;
}

LPWSTR ANSIToUnicode(char* src)
{
	if (!src) return 0;
	int srcLen = strlen(src);
	if (!srcLen)
	{
		wchar_t* w = new wchar_t[1];
		w[0] = 0;
		return w;
	}

	int requiredSize = MultiByteToWideChar(GetACP(), 0, src, srcLen, 0, 0);

	if (!requiredSize)
	{
		return 0;
	}

	wchar_t* w = new wchar_t[requiredSize + 1];
	w[requiredSize] = 0;

	int retval = MultiByteToWideChar(GetACP(), 0, src, srcLen, w, requiredSize);
	if (!retval)
	{
		delete[] w;
		return 0;
	}

	return w;
}

LPSTR UnicodeToANSI(LPCWSTR src)
{
	if (!src) return 0;
	int srcLen = wcslen(src);
	if (!srcLen)
	{
		char* x = new char[1];
		x[0] = '\0';
		return x;
	}

	int requiredSize = WideCharToMultiByte(GetACP(), WC_COMPOSITECHECK, src, srcLen, 0, 0, 0, 0);

	if (!requiredSize)
	{
		return 0;
	}

	char* x = new char[requiredSize + 1];
	x[requiredSize] = 0;

	int retval = WideCharToMultiByte(GetACP(), WC_COMPOSITECHECK, src, srcLen, x, requiredSize, 0, 0);

	if (!retval)
	{
		delete[] x;
		return 0;
	}

	return x;
}
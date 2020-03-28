#include "pch.h"

void UnicodeAnsi(LPCTSTR source, LPCTSTR destination);
void AnsiUnicode(LPCTSTR source, LPCTSTR destination);

int main(int argc, TCHAR* argv[])
{
	INT numberOfArguments = argc;
	TCHAR argument;
	if (numberOfArguments == 1)
	{
		printf("No arguments!\n");
		return 0;
	}
	argument = *argv[1];
	if (argument == 'a') {
		LPCTSTR _sourceFileName = (LPCTSTR)argv[2];
		LPCTSTR _destinationFileName = (LPCTSTR)argv[3];
		printf((const char*)_sourceFileName);
		printf((const char*)_destinationFileName);
		UnicodeAnsi(_sourceFileName, _destinationFileName);
	}
	else if (argument == 'u') {
		LPCTSTR _sourceFileName = (LPCTSTR)argv[2];
		LPCTSTR _destinationFileName = (LPCTSTR)argv[3];
		printf((const char*)_sourceFileName);
		printf((const char*)_destinationFileName);
		AnsiUnicode(_sourceFileName, _destinationFileName);
	}
	else printf("Invalid argument");
}

void UnicodeAnsi(LPCTSTR source, LPCTSTR destination)
{
	HANDLE file;
	HANDLE fileDestination;
	file = CreateFile(source, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	fileDestination = CreateFile(destination, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) return;
	if (fileDestination == INVALID_HANDLE_VALUE) return;
	DWORD sizeOfHandle = GetFileSize(file, NULL);
	LPWSTR readBuffer = new WCHAR[4];
	DWORD bytesHaveRead = -1;
	while (bytesHaveRead != 0) {
		ReadFile(file, readBuffer, 4, &bytesHaveRead, NULL);
		int requiredSize = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (LPCWSTR)readBuffer, wcslen(readBuffer), 0, 0, 0, 0);
		LPSTR result = new CHAR[requiredSize];
		int returnValue = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (LPCWSTR)readBuffer, wcslen(readBuffer), result, requiredSize, 0, 0);
		DWORD bytesToWrite = strlen(result) * sizeof(CHAR);
		DWORD bytesWritten;
		WriteFile(fileDestination, result, bytesToWrite, &bytesWritten, NULL);
	}
	CloseHandle(file);
	CloseHandle(fileDestination);
}

void AnsiUnicode(LPCTSTR source, LPCTSTR destination)
{
	HANDLE file;
	HANDLE fileDestination;
	file = CreateFile(source, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	fileDestination = CreateFile(destination, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) return;
	if (fileDestination == INVALID_HANDLE_VALUE) return;
	DWORD sizeOfHandle = GetFileSize(file, NULL);
	LPSTR readBuffer = new CHAR[4];
	DWORD bytesHaveRead = -1;
	while (bytesHaveRead != 0)
	{
		ReadFile(file, readBuffer, 4, &bytesHaveRead, NULL);
		int requiredSize = MultiByteToWideChar(CP_ACP, 0, readBuffer, strlen(readBuffer), 0, 0);
		LPWSTR result = new WCHAR[requiredSize];
		int returnValue = MultiByteToWideChar(CP_ACP, 0, readBuffer, strlen(readBuffer), result, requiredSize);
		DWORD bytesToWrite = wcslen(result) * sizeof(WCHAR);
		DWORD bytesWritten;
		WriteFile(fileDestination, result, bytesToWrite, &bytesWritten, NULL);
	}
	CloseHandle(file);
	CloseHandle(fileDestination);
}

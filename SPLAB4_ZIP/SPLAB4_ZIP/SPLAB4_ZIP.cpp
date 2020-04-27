#include "pch.h"

const TCHAR ARCHIVER_PATH[] = L"C:\\Program Files (x86)\\7-Zip\\7z.exe";

void readline(TCHAR szBuffer[], DWORD nBufferSize);
void archive(LPCTSTR lpPathName, LPCTSTR lpOutArchivePath);
void extract(LPCTSTR lpArchivePath, LPCTSTR lpOutPathName);
void execute(LPTSTR command);
void printErrorMessage();

int main()
{
	INT operation;
	TCHAR InputPath[MAX_PATH];
	TCHAR OutputPath[MAX_PATH];
	while (true)
	{
		_tprintf(L"Archiver\n");
		_tprintf(L"1. Eject.\n");
		_tprintf(L"2. Archive.\n");
		_tprintf(L"0. Exit.\n");
		_tprintf(L">> ");
		_tscanf(L"%d", &operation);
		switch (operation)
		{
		case 0:
			return 0;
		case 1:
			_tprintf(L"Path to an archive: ");
			readline(InputPath, MAX_PATH);
			_tprintf(L"Eject into: ");
			readline(OutputPath, MAX_PATH);
			extract(InputPath, OutputPath);
			break;
		case 2:
			_tprintf(L"Path to folder of file you want to archive: ");
			readline(InputPath, MAX_PATH);
			_tprintf(L"Path to archive: ");
			readline(OutputPath, MAX_PATH);
			archive(InputPath, OutputPath);
			break;
		default:
			_tprintf(L"There are not such a command!\n");
			break;
		}
	}
	return 0;
}
void readline(TCHAR szBuffer[], DWORD nBufferSize)
{
	DWORD i = 0;
	TCHAR ch;
	while (isspace(ch = getchar()));
	while (ch != '\n')
	{
		if (i < nBufferSize)
			szBuffer[i++] = ch;
		ch = getchar();
	}
	szBuffer[i] = '\0';
}

void archive(LPCTSTR lpPathName, LPCTSTR lpOutArchivePath)
{
	INT iMaxLen = lstrlen(lpPathName) + lstrlen(lpOutArchivePath) +
		lstrlen(ARCHIVER_PATH) + 50;
	wchar_t* buf = new wchar_t[iMaxLen];
	_stprintf(buf, TEXT("\"%s\" a \"%s\" \"%s\" -y"), ARCHIVER_PATH,
		lpOutArchivePath, lpPathName);
	execute(buf);
	return;
}
void extract(LPCTSTR lpArchivePath, LPCTSTR lpOutPathName)
{
	INT iMaxLen = lstrlen(lpArchivePath) + lstrlen(lpOutPathName) +
		lstrlen(ARCHIVER_PATH) + 50;
	wchar_t* buf = new wchar_t[iMaxLen];
	swprintf(buf, iMaxLen, L"\"%s\" x \"%s\" -o\"%s\" *.* -r -y", ARCHIVER_PATH,
		lpArchivePath, lpOutPathName);
	execute(buf);
	return;
}
void execute(LPTSTR command)
{
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;
	if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0))
	{
		printErrorMessage();
		return;
	}
	if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0))
	{
		printErrorMessage();
		return;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.hStdError = hWritePipe;
	si.dwFlags |= STARTF_USESTDHANDLES;
	ZeroMemory(&pi, sizeof(pi));
	if (CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) ==
		NULL)
	{
		printErrorMessage();
		return;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(hReadPipe);

	DWORD dwRead;
	BOOL bSuccess = FALSE;
	CHAR chRead[1024];
	bSuccess = ReadFile(hReadPipe, chRead, 1024, &dwRead, NULL);
	chRead[dwRead] = '\0';
	if (strstr(chRead, "Îøèáêà") != NULL || strstr(chRead, "WARNING") != NULL)
	{
		_tprintf(L"%s", chRead);
	}
	else
	{
		_tprintf(L"Successfull!\n");
	}
	CloseHandle(hWritePipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return;
}
void printErrorMessage()
{
	HRESULT hResult = GetLastError();
	LPTSTR errorText = NULL;
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,
		0,
		NULL);
	if (NULL != errorText)
	{
		wprintf(L"%s", errorText);
		LocalFree(errorText);
	}
}
#include "pch.h"
#include <direct.h>

#define MAX_LENGTH 100

void printDirectory(LPTSTR workDir);
void printFile(LPTSTR workDir);
void changeWorkDirectory(LPTSTR workDir, LPTSTR changedDir);
BOOL copyFile(LPTSTR workDir);
void deleteFile(LPTSTR workDir);
void createDirectory(LPTSTR workDir);

int main(int argc, LPTSTR argv[])
{
	INT command;
	TCHAR root[MAX_LENGTH];
	GetCurrentDirectory(MAX_LENGTH, root);
	TCHAR workDir[MAX_LENGTH], changedDir[MAX_LENGTH];
	BOOL result;
	lstrcpy(workDir, root);
	for (;;)
	{
		printf("Options:\n1. Print directory info.\n2. Change directory.\n3. Create directory or file.\n4. File information.\n5. Copy file\n6. Delete file of empty directory.\n7. Exit.\nYour choice: ");
		scanf("%i", &command);
		switch (command)
		{
		case 1: printDirectory(workDir); break;
		case 2: changeWorkDirectory(workDir, changedDir); break;
		case 3: createDirectory(workDir); break;
		case 4: printFile(workDir); break;
		case 5: result = copyFile(workDir); break;
		case 6: deleteFile(workDir); break;
		case 7: return 0; break;
		default:
			puts("Wrong command!"); break;
		}
	}
}

void printDirectory(LPTSTR workDir) {
	TCHAR directoryPath[MAX_LENGTH * 2];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	LARGE_INTEGER FileSize;
	SYSTEMTIME st;
	_tprintf(L"\nCurrent directory - %s\n", workDir);
	lstrcpy(directoryPath, workDir);
	lstrcat(directoryPath, L"\\*.*");
	hFind = FindFirstFile(directoryPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		_tprintf(L"There are not such a directory!\n");
	}
	else {
		printf("\n ------------------------------------------------------------------------------------------------------ ------------ -------------------- ------------\n");
		printf("| %-100s | %-10s | %-18s | %-10s |\n", "Name", "Type", "Date", "Size");
		printf(" ------------------------------------------------------------------------------------------------------ ------------ -------------------- ------------\n");
		do
		{

			if (wcscmp(FindFileData.cFileName, L".") != 0 && wcscmp(FindFileData.cFileName, L"..") != 0)
			{
				FileTimeToSystemTime(&FindFileData.ftCreationTime, &st);
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					_tprintf(L" %-100s   %-10s   %2ld:%2ld:%2ld, %2ld-%2ld-%4ld   %-10s \n", FindFileData.cFileName, L"Folder", st.wSecond,
						st.wMinute, st.wHour, st.wDay, st.wMonth, st.wYear, L"");
				else
				{
					FileSize.HighPart = FindFileData.nFileSizeHigh;
					FileSize.LowPart = FindFileData.nFileSizeLow;
					_tprintf(L" %-100s   %-10s   %2ld:%2ld:%2ld, %2ld-%2ld-%4ld   %-10ld \n", FindFileData.cFileName, L"File", st.wSecond,
						st.wMinute, st.wHour, st.wDay, st.wMonth, st.wYear, FileSize);
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);
		printf("\n");
		FindClose(hFind);
	}
 }

void printFile(LPTSTR workDir) {
	TCHAR directoryPath[MAX_LENGTH * 2];
	TCHAR fileName[MAX_LENGTH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind; 
	LARGE_INTEGER FileSize;
	SYSTEMTIME st;
	_tprintf(L"\nCurrent directory - %s\n", workDir);
	lstrcpy(directoryPath, workDir);
	printf("Input file name: ");
	_tscanf(L"%s", &fileName);
	lstrcat(directoryPath, L"\\");
	lstrcat(directoryPath, fileName);
	hFind = FindFirstFile(directoryPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		_tprintf(L"There are not such a file!\n");
	}
	else {
		printf("\n ------------------------------------------------------------------------------------------------------ ------------ -------------------- ------------\n");
		printf("| %-100s | %-10s | %-18s | %-10s |\n", "Name", "Type", "Date", "Size");
		printf(" ------------------------------------------------------------------------------------------------------ ------------ -------------------- ------------\n");
		do
		{
			if (wcscmp(FindFileData.cFileName, L".") != 0 && wcscmp(FindFileData.cFileName, L"..") != 0)
			{
				FileTimeToSystemTime(&FindFileData.ftCreationTime, &st);
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					_tprintf(L"That's not a file!\n");
				else
				{
					FileSize.HighPart = FindFileData.nFileSizeHigh;
					FileSize.LowPart = FindFileData.nFileSizeLow;
					_tprintf(L" %-100s   %-10s   %2ld:%2ld:%2ld, %2ld-%2ld-%4ld   %-10ld \n", FindFileData.cFileName, L"File", st.wSecond,
						st.wMinute, st.wHour, st.wDay, st.wMonth, st.wYear, FileSize);
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);
		printf("\n");
		FindClose(hFind);
	}
}

void changeWorkDirectory(LPTSTR workDir, LPTSTR changedDir) {
	_tprintf(L"Current directory - %s\n", workDir);
	_tprintf(L"Input new work directory path: ");
	_tscanf(L"%s", changedDir);
	GetCurrentDirectory(MAX_LENGTH, workDir);
	SetCurrentDirectory(changedDir);
	lstrcpy(workDir, changedDir);
	_tprintf(L"Current directory - %s\n", workDir);
}

BOOL copyFile(LPTSTR workDir)
{
	TCHAR fileFirstName[MAX_LENGTH];
	TCHAR filePath[MAX_LENGTH * 2];
	TCHAR folderPath[MAX_LENGTH];
	TCHAR fileName[MAX_LENGTH];
	TCHAR fullPath[MAX_LENGTH * 2];
	WIN32_FIND_DATA findFileData;
	WIN32_FIND_DATA findFolderData;
	HANDLE hFile;
	HANDLE hFolder;

	_tprintf(L"\nCurrent directory - %s\n", workDir);
	printf("Input file name, you want to copy: ");
	_tscanf(L"%s", &fileFirstName);
	lstrcpy(filePath, workDir);
	lstrcat(filePath, L"\\");
	lstrcat(filePath, fileFirstName);

	hFile = FindFirstFile(filePath, &findFileData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(L"There are not such a file!\n");
	}
	else
	{
		do
		{
			if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0)
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					_tprintf(L"That's not a file!\n");
				else
				{
					printf("Input a folder name you want to copy in: ");
					_tscanf(L"%s", &folderPath);
					lstrcpy(fullPath,folderPath);
					lstrcat(folderPath, L"\\*.*");
					hFolder = FindFirstFile(folderPath, &findFolderData);
					if (hFolder == INVALID_HANDLE_VALUE)
					{
						_tprintf(L"There are not such a folder!\n");
					}
					else {
						do
						{
							_tprintf(L"Input file name (format - file_name.file_extension): ");
							_tscanf(L"%s", &fileName);
							lstrcat(fullPath, L"\\");
							lstrcat(fullPath, fileName);
							if (CopyFile(filePath, fullPath, FALSE))
							{
								printf("Successfully!\n");
								return true;
							}
							printf("Unsuccessfully!\n");
							return false;
						} while (FindNextFile(hFolder, &findFolderData) != 0);
					}
				}
			}
		} while (FindNextFile(hFile, &findFileData) != 0);
		printf("\n");
		FindClose(hFile);
	}
}
void createDirectory(LPTSTR workDir) {
	INT choice;
	TCHAR newFolderPath[MAX_LENGTH * 2];
	TCHAR newFolderName[MAX_LENGTH];
	TCHAR newFileName[MAX_LENGTH];
	_tprintf(L"\nCurrent directory - %s\n", workDir);
	lstrcpy(newFolderPath, workDir);
	_tprintf(L"What do you want to create:\n1. Folder.\n2. File.\nYour choice: ");
	_tscanf(L"%i", &choice);
	if (choice == 1)
	{
		_tprintf(L"Input folder name you want to create: ");
		_tscanf(L"%s", &newFolderName);
		lstrcat(newFolderPath, L"\\");
		lstrcat(newFolderPath, newFolderName);
		if (CreateDirectory(newFolderPath, NULL) != 0)
		{
			printf("Successfully!\n");
		}
		else
		{
			printf("Unsuccessfully!\n");
		}
	}
	else if (choice == 2)
	{
		_tprintf(L"Input file name you want to create (format - file_name.file_extension): ");
		_tscanf(L"%s", &newFileName);
		lstrcat(newFolderPath, L"\\");
		lstrcat(newFolderPath, newFileName);
		if (CreateFile(newFolderPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0) != INVALID_HANDLE_VALUE)
		{
			printf("Successfully!\n");
		}
		else
		{
			printf("Unsuccessfully!\n");
		}
	}
	else printf("The're not such a option!\n");
}

void deleteFile(LPTSTR workDir) 
{
	INT choice;
	TCHAR newFolderPath[MAX_LENGTH * 2];
	TCHAR newFolderName[MAX_LENGTH];
	TCHAR newFileName[MAX_LENGTH];
	_tprintf(L"\nCurrent directory - %s\n", workDir);
	lstrcpy(newFolderPath, workDir);
	_tprintf(L"What do you want to delete:\n1. Folder.\n2. File.\nYour choice: ");
	_tscanf(L"%i", &choice);
	if (choice == 1)
	{
		_tprintf(L"Input folder name you want to delete: ");
		_tscanf(L"%s", &newFolderName);
		lstrcat(newFolderPath, L"\\");
		lstrcat(newFolderPath, newFolderName);
		if (RemoveDirectory(newFolderPath) != 0)
		{
			printf("Successfully!\n");
		}
		else
		{
			printf("Unsuccessfully!\n");
		}
	}
	else if (choice == 2)
	{
		_tprintf(L"Input file name you want to create (format - file_name.file_extension): ");
		_tscanf(L"%s", &newFileName);
		lstrcat(newFolderPath, L"\\");
		lstrcat(newFolderPath, newFileName);
		if (DeleteFile(newFolderPath) != 0)
		{
			printf("Successfully!\n");
		}
		else
		{
			printf("Unsuccessfully!\n");
		}
	}
	else printf("The're not such a option!\n");
}

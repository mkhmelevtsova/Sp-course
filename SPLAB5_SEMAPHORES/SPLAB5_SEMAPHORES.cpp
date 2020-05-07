#include "pch.h"

#define THREAD_COUNT 64
#define SEMAPHORE_COUNT 20
#define FILE_NAME _T("resultFile.txt")

HANDLE semaphore;
HANDLE file;

DWORD Function(LPVOID);

int main()
{
	file = CreateFile(FILE_NAME, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);	
	if (file == INVALID_HANDLE_VALUE)
	{
		printf("Creating file error");
		CloseHandle(file);
		return 0;
	}
	semaphore = CreateSemaphore(NULL, SEMAPHORE_COUNT, SEMAPHORE_COUNT, _T("newSemaphore"));
	HANDLE threads[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; i++)
		threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Function, NULL, NULL, 0);
	WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);
	for (int i = 0; i < THREAD_COUNT; i++)
		CloseHandle(threads[i]);
	CloseHandle(semaphore);
	CloseHandle(file);
	return 0;
}

DWORD Function(LPVOID lpParam)
{
	DWORD currentThreadId = GetCurrentThreadId();
	srand(currentThreadId);
	int random = rand() % 2000 + 1000;
	Sleep(random);
	if (WaitForSingleObject(semaphore, INFINITE) == WAIT_FAILED) return 0;
	DWORD DWsize;
	CHAR str[100];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(str, "ID - %4d %d:% 02d: % 02d : % 03d\r\n", currentThreadId,random,st.wMinute,st.wSecond,st.wMilliseconds);
		WriteFile(file, &str, strlen(str), &DWsize, NULL);
	printf("%s", str);
	random = rand() % 2000 + 1000;
	Sleep(random);
	if (FlushFileBuffers(file) == 0) return 0;
	if (ReleaseSemaphore(semaphore, 1, NULL) == 0) return 0;
}
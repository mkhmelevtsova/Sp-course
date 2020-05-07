#include "pch.h"

int main()
{
	HANDLE mutex = CreateMutex(NULL, TRUE, L"NewMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		_tprintf(L"Mutex already created!\nProgramm will be closed!\nPress any key.\n");
		getchar();
		return 0;
	}
	else
	{
		_tprintf(L"Programm is running!\nPress any key to close mutex and programm.\n");
		getchar();
		CloseHandle(mutex);
		return 0;
	}
}
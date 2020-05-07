#include "pch.h"

#define ARR_SIZE 5000000

void SumCritical(LPVOID);
void AverageCritical(LPVOID);
void MaxMinCritical(LPVOID);
void Sum(LPVOID);
void Average(LPVOID);
void MaxMin(LPVOID);

int arr[ARR_SIZE];
CRITICAL_SECTION CS;

int main()
{
	for (int i = 0; i < ARR_SIZE; i++)
		arr[i] = i;
	HANDLE threadsCritical[3];
	HANDLE threads[3];
	int k = 0;
	_tprintf(_T("Critical Section:\n"));
	InitializeCriticalSection(&CS);
	threadsCritical[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SumCritical, NULL, NULL, 0);
	threadsCritical[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AverageCritical, NULL, NULL, 0);
	threadsCritical[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MaxMinCritical, NULL, NULL, 0);
	WaitForMultipleObjects(3, threadsCritical, TRUE, INFINITE);
	_tprintf(_T("\nMulti Thread:\n"));
	threads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Sum, NULL, NULL, 0);
	threads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Average, NULL, NULL, 0);
	threads[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MaxMin, NULL, NULL, 0);
	WaitForMultipleObjects(3, threads, TRUE, INFINITE);
	DeleteCriticalSection(&CS);
	for (int i = 0; i < 3; i++)
	{
		CloseHandle(threadsCritical[i]);
		CloseHandle(threads[i]);
	}
	return 0;
}

void SumCritical(LPVOID lpParam)
{
	LARGE_INTEGER frequency, start, finish;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	long long sum = 0;
	for (int i = 0; i < ARR_SIZE; i++)
	{
		EnterCriticalSection(&CS);
		sum += arr[i];
		LeaveCriticalSection(&CS);
	}
	QueryPerformanceCounter(&finish);
	_tprintf(_T("Sum: %i\n"), sum);
	_tprintf(_T("Time: %f\n\n"), (finish.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart);
}

void AverageCritical(LPVOID lpParam)
{
	LARGE_INTEGER frequency, start, finish;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	long long sum = 0;
	for (int i = 0; i < ARR_SIZE; i++)
	{
		EnterCriticalSection(&CS);
		sum += arr[i];
		LeaveCriticalSection(&CS);
	}
	QueryPerformanceCounter(&finish);
	_tprintf(_T("Average: %.1f\n"), (double)sum / ARR_SIZE);
	_tprintf(_T("Time: %f\n\n"), (finish.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart);
}

void MaxMinCritical(LPVOID lpParam)
{
	LARGE_INTEGER frequency, start, finish;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	int min = arr[0], max = arr[1];
	for (int i = 0; i < ARR_SIZE; i++)
	{
		EnterCriticalSection(&CS);
		if (arr[i] > max)
			max = arr[i];
		if (arr[i] < min)
			min = arr[i];
		LeaveCriticalSection(&CS);
	}
	QueryPerformanceCounter(&finish);
	_tprintf(_T("Min: %i\n"), min);
	_tprintf(_T("Max: %i\n"), max);
	_tprintf(_T("Time: %f\n\n"), (finish.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart);
}

void Sum(LPVOID lpParam)
{
	LARGE_INTEGER frequency, start, finish;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	long long sum = 0;
	for (int i = 0; i < ARR_SIZE; i++)
		sum += arr[i];
	QueryPerformanceCounter(&finish);
	_tprintf(_T("Sum: %i\n"), sum);
	_tprintf(_T("Time: %f\n\n"), (finish.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart);
}

void Average(LPVOID lpParam)
{
	LARGE_INTEGER frequency, start, finish;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	long long sum = 0;
	for (int i = 0; i < ARR_SIZE; i++)
		sum += arr[i];
	QueryPerformanceCounter(&finish);
	_tprintf(_T("Average: %.1f\n"), (double)sum / ARR_SIZE);
	_tprintf(_T("Time: %f\n\n"), (finish.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart);
}

void MaxMin(LPVOID lpParam)
{
	LARGE_INTEGER frequency, start, finish;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	int min = arr[0], max = arr[1];
	for (int i = 0; i < ARR_SIZE; i++)
	{
		if (arr[i] > max)
			max = arr[i];
		if (arr[i] < min)
			min = arr[i];
	}
	QueryPerformanceCounter(&finish);
	_tprintf(_T("Min: %i\n"), min);
	_tprintf(_T("Max: %i\n"), max);
	_tprintf(_T("Time: %f\n\n"), (finish.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart);
}
#include "pch.h"

struct INTArray
{
    INT arrayLenght;
    INT* array;
};

DWORD tlsIndex;

INTArray* randomArray(DWORD arrSize);
DWORD WINAPI ThreadFunction(LPVOID arrayParameter);
void FunctionXorToArray();
INT FunctionSumToArray();

int main()
{
    INT numberOfThreads;
    HANDLE* threads;
    tlsIndex = TlsAlloc();
    _tprintf(L"Input number of threads: ");
    _tscanf(L"%i", &numberOfThreads);
    threads = new HANDLE[numberOfThreads];
    for (int i = 0; i < numberOfThreads; i++) threads[i] = CreateThread(NULL, 0, ThreadFunction, (LPVOID)randomArray(5 + rand() % 6), 0, NULL);
    WaitForMultipleObjects((DWORD)numberOfThreads, threads, TRUE, INFINITE);
    for (int i = 0; i < numberOfThreads; i++) CloseHandle(threads[i]);
    delete[] threads;
    TlsFree(tlsIndex);
    return 0;
}

DWORD WINAPI ThreadFunction(LPVOID arrayParameter) 
{
    INTArray* arrayPointer = (INTArray*)arrayParameter;
    TlsSetValue(tlsIndex, arrayParameter);
    FunctionXorToArray();
    _tprintf(L"Thread #%u, sum = %u\n", GetCurrentThreadId(), FunctionSumToArray());
    delete[] arrayPointer->array;
    return 0;
}

INTArray* randomArray(DWORD arrSize)
{
    INTArray* arr = new INTArray;
    arr->arrayLenght = arrSize;
    arr->array = new INT[arrSize];
    for (DWORD i = 0; i < arrSize; i++)
    {
        arr->array[i] = 10 + rand() % 100;
    }
    return arr;
}

void FunctionXorToArray()
{
    INTArray* arr = (INTArray*)TlsGetValue(tlsIndex);
    for (int i = 0; i < arr->arrayLenght - 1; i++)
    {
        arr->array[i] = arr->array[i] ^ arr->array[i + 1];
    }
    arr->arrayLenght--;
}

INT FunctionSumToArray()
{
    INTArray* arr = (INTArray*)TlsGetValue(tlsIndex);
    INT sum = 0;
    for (int i = 0; i < arr->arrayLenght; i++)
    {
        sum += arr->array[i];
    }
    return sum;
}
#include "pch.h"

void ShowError();
void SystemInformation();

int main(int argc, char* argv[])
{
	int i = argc;
	if (i == 1)
	{
		printf("No arguments\n");
	}
	else
	{
		switch (*argv[1])
		{
		case 'e':
			if (!GetProcessId(NULL))
			{
				ShowError();
			}
			break;
		case 's':
			SystemInformation();
			break;
		default:
			printf("Invalid argument\n");
			break;
		}
		i--;

	}
	return 0;
}
void SystemInformation()
{
	MEMORYSTATUS status;
	GlobalMemoryStatus(&status);
	printf("\nMemory Information:\n");
	printf("Length: %lu\n", status.dwLength);
	printf("Memory Load: %lu percentage \n", status.dwMemoryLoad);
	printf("Total Phys: %lu B\n", (DWORD)status.dwTotalPhys);
	printf("Avail Phys: %lu B\n", (DWORD)status.dwAvailPhys);
	printf("Total Page File: %lu B\n", status.dwTotalPageFile);
	printf("Avail Page File: %lu B\n", status.dwAvailPageFile);
	printf("Total Virtual: %lu B\n", status.dwTotalVirtual);
	printf("Avail Virtual: %lu B\n", status.dwAvailVirtual);

	SYSTEM_INFO info;
	GetSystemInfo(&info);
	printf("\nSystem Information:\n");
	printf("Processor Architecture: 0x%x\n", info.dwOemId);
	printf("Page Size: 0x%x\n", info.dwPageSize);
	printf("Minimum Application Address: %lu\n", (DWORD)info.lpMinimumApplicationAddress);
	printf("Maximum Application Address: %lu\n", (DWORD)info.lpMaximumApplicationAddress);
	printf("Active ProcessorMask: %lu\n", info.dwActiveProcessorMask);
	printf("Number of Processors: %lu\n", info.dwNumberOfProcessors);
	printf("Processor Type: %lu\n", info.dwProcessorType);
	printf("Allocation Granularity: 0x%x\n", info.dwAllocationGranularity);
	printf("Processor Level: %lu\n", info.wProcessorLevel);
	printf("Processor Revision: %lu\n", info.wProcessorRevision);
}
void ShowError()
{
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("%s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
}
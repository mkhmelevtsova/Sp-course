#include "pch.h"

struct Header {
	INT CountOfRecords;
	INT CountOfFilledRecords;
	UINT Size;
};

struct Record {
	INT Number;
	FILETIME CreationTime;
	INT ChangesCount;
	WCHAR Text[80];
};

void checkError(DWORD messageID);
void printInfo(Header header, Record* records);
void WriteToFile(HANDLE file, Header header, Record* records, OVERLAPPED overlapped_structure);


int main(int argc, LPTSTR argv[])
{
	LPCWSTR fileName = L"result.txt";
	Header headerForCreation;
	Record* recordsForCreation;
	Header header;
	Record* records;
	DWORD numberOfBytesWritten;

	OVERLAPPED overlapped_structure;
	memset(&overlapped_structure, 0, sizeof(overlapped_structure));
	overlapped_structure.Offset = 0;
	overlapped_structure.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	while (true)
	{
		overlapped_structure.Offset = 0;
	   	HANDLE mainFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
		if (mainFile == INVALID_HANDLE_VALUE)
		{
			HANDLE mainFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
			if (mainFile == INVALID_HANDLE_VALUE) return 0;
			printf("File result.txt created!\n");
			printf("Input structures number: ");
			int countOfRecords;
			scanf_s("%d", &countOfRecords);
			headerForCreation.CountOfRecords = countOfRecords;
			headerForCreation.Size = sizeof(Header) + (sizeof(Record) * countOfRecords);
			recordsForCreation = (Record*)malloc(sizeof(Record) * countOfRecords);
			INT countOfFilledRecords = 0;
			for (int i = 0; i < countOfRecords; i++) 
			{
				recordsForCreation[i].Number = i + 1;
				recordsForCreation[i].ChangesCount = 0;
				GetSystemTimeAsFileTime(&recordsForCreation[i].CreationTime);
				lstrcpynW(recordsForCreation[i].Text, L"Default text", 80);
				countOfFilledRecords++;
			}
			headerForCreation.CountOfFilledRecords = countOfFilledRecords;
			WriteToFile(mainFile, headerForCreation, recordsForCreation, overlapped_structure);

			printf("Here he have:\n");
			printInfo(headerForCreation, recordsForCreation);

			CloseHandle(mainFile);
		}
		else {
			overlapped_structure.Offset = 0;
			ReadFile(mainFile, &header, sizeof(Header), &numberOfBytesWritten, &overlapped_structure);
			overlapped_structure.Offset = overlapped_structure.InternalHigh;
			records = (Record*)malloc(sizeof(Record) * header.CountOfRecords);
			for (int i = 0; i < header.CountOfRecords; i++)
			{
				ReadFile(mainFile, &records[i], sizeof(records[i]), &numberOfBytesWritten, &overlapped_structure);
				overlapped_structure.Offset += overlapped_structure.InternalHigh;
			}
			while (true) 
			{
				printf("Options:\n1. Show info.\n2. Delete record data.\n3. Modify records.\n4. Exit\nYour choice: ");
				int choice;
				scanf_s("%d", &choice);
 				switch (choice) 
				{
					case 1: 
					{
						printInfo(header, records);
						break;
					}
					case 2: 
					{
						printf("Input number of record you want to delete: ");
						int chosenRecord;
						scanf_s("%d", &chosenRecord);
						if (chosenRecord > header.CountOfRecords || chosenRecord < 1)
						{
							printf("Record with such number doesn't exist.\n");
							break;
						}
						lstrcpynW(records[chosenRecord - 1].Text, L"", 80);
						records[chosenRecord - 1].ChangesCount++;
						header.CountOfFilledRecords--;
						WriteToFile(mainFile, header, records, overlapped_structure);
						break;
					}
					case 3: 
					{
						printf("Input number of record you want to modify: ");
						int chosenRecord;
						scanf_s("%d", &chosenRecord);
						if (chosenRecord > header.CountOfRecords || chosenRecord < 1)
						{
							printf("Record with such number doesn't exist.\n");
							break;
						}
						printf("Input text (only 80 characters will be recorded): ");
						LPWSTR inputText = new WCHAR[80];
						wscanf(L"%s", inputText);
						if ((LPWSTR)inputText == (LPWSTR)L"")
						{
							header.CountOfFilledRecords--;
						}
						LPCWSTR text = inputText;
						lstrcpynW(records[chosenRecord - 1].Text, text, 80);
						records[chosenRecord - 1].ChangesCount++;
						WriteToFile(mainFile, header, records, overlapped_structure);
						break;
					}
					case 4: 
					{
						CloseHandle(mainFile);
						return 0;
						break;
					}
					default: 
					{
						printf("There are not such a command!\n");
						break;
					}
				}
			}
		}
	}
}

void WriteToFile(HANDLE file, Header header, Record* records, OVERLAPPED overlapped_structure)
{
	DWORD numberOfBytesWritten;
	overlapped_structure.Offset = 0;
	WriteFile(file, &header, sizeof(header), &numberOfBytesWritten, &overlapped_structure);
	overlapped_structure.Offset = overlapped_structure.InternalHigh;
	for (int i = 0; i < header.CountOfRecords; i++)
	{
		WriteFile(file, &records[i], sizeof(records[i]), &numberOfBytesWritten, &overlapped_structure);
		overlapped_structure.Offset += overlapped_structure.InternalHigh;
	}
}

void printInfo(Header header, Record* records) {
	printf(" ------------------------- ------------\n");
	printf("| %-23s | %-10s |\n", "Count of filled records", "Size");
	wprintf(L"| %-23i | %-10lu | \n", header.CountOfFilledRecords, header.Size);
	printf(" ------------------------- ------------\n\n");

	printf(" ----- -------------------- --------------- ----------------------------------------------------------------------------------\n");
	printf("| %-3s | %-18s | %-13s | %-80s |\n", "Num", "Creation time", "Changes count", "Text");
	for (int i = 0; i < header.CountOfRecords; i++) 
	{
		SYSTEMTIME stUTC, stLocal;
		FileTimeToSystemTime(&records[i].CreationTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		LPWSTR dateTime = new WCHAR[20];
		wsprintf(dateTime, L"%02d/%02d/%d  %02d:%02d", stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute);
		wprintf(L"| %-3i | %-18s | %-13i | %-80s |\n", records[i].Number, dateTime, records[i].ChangesCount, &records[i].Text);
	}
	printf(" ----- -------------------- --------------- ----------------------------------------------------------------------------------\n\n");
}

void checkError(DWORD messageID) {
	LPVOID lpMsg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		messageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsg,
		0,
		NULL);
	printf(("%s"), lpMsg);
}
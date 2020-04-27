#include "pch.h"

void PrintSubkeysList();
void PrintKeyParametres();
void SearchString();
void KeyAnalyser(HKEY initialSection, TCHAR keyPath[], LPTSTR searchValue);
void SaveKey();

int main(int argc, LPTSTR argv[])
{
    INT option;
    while (true) {
        _tprintf(L"\nSelect option:\n1. Print subkeys list.\n2. Print key's parameters with values and types.\
                    \n3. Search string in registry.\n4. Export key in file.\n5. Exit.\nOption: ");
        _tscanf(L"%i", &option);
        switch (option)
        {
            case 1:
            {
                PrintSubkeysList();
                break;
            }
            case 2:
            {
                PrintKeyParametres();
                break;
            }
            case 3:
            {
                SearchString();
                break;
            }
            case 4:
            {
                SaveKey();
                break;
            }
            case 5:
            {
                return EXIT_SUCCESS;
                break;
            }
            default:
            {
                _tprintf(L"There is not such an option!\n");
                break;
            }
        }
    }
	return EXIT_SUCCESS;
}

void PrintSubkeysList()
{
    LPTSTR keyName;
    HKEY    hKey = 0;
    LONG    subkeyResultCode;
    DWORD   dwValue = 256;
    DWORD   dwIndex = 0;
    LPTSTR subkeyName = new TCHAR[256];
    LPTSTR subKey = new TCHAR[256];
    INT option;
    _tprintf(L"\nSelect initial section:\n1. HKEY_CLASSES_ROOT\n2. HKEY_CURRENT_CONFIG\n\
                3. HKEY_CURRENT_USER\n4. HKEY_LOCAL_MACHINE\n5. HKEY_USERS\nYour option: ");
    _tscanf(L"%i", &option);
    HKEY initialSection = option == 1 ? HKEY_CLASSES_ROOT : option == 2 ? HKEY_CURRENT_CONFIG : option == 3 ? HKEY_CURRENT_USER : option == 4 ? HKEY_LOCAL_MACHINE : option == 5 ? HKEY_USERS : NULL;
    if (initialSection != NULL) {
        _tprintf(L"Input keys path (if you want to print initial section's subkeys list, enter 'd'): ");
        _tscanf(L"%s", subKey);
        if (lstrcmpW(subKey, L"d") == 0) subKey = (LPTSTR)L"";
        LONG lRes = RegOpenKeyEx(initialSection, subKey, NULL, KEY_ENUMERATE_SUB_KEYS, &hKey);
        if (lRes != ERROR_SUCCESS)
        {
            _tprintf(L"Error");
        }
        else
        {
            _tprintf(L"\n");
            do
            {
                dwValue = 256;
                subkeyResultCode = RegEnumKeyEx(hKey, dwIndex, subkeyName, &dwValue, NULL, NULL, NULL, NULL);
                if (subkeyResultCode == ERROR_SUCCESS)
                {
                    _tprintf(L"%s\n", subkeyName);
                }
                else
                {
                    keyName = (LPTSTR)L"Error!";
                }
                dwIndex++;
            } while (subkeyResultCode != ERROR_NO_MORE_ITEMS);
        }
        RegCloseKey(hKey);
    }
}

void PrintKeyParametres()
{
    LPTSTR keyName;
    HKEY    hKey = 0;
    LONG    subkeyResultCode;
    DWORD   dwValue = 256;
    DWORD   dwIndex = 0;
    LPTSTR parameterName = new TCHAR[256];
    DWORD parameterType = 0;
    BYTE parameterValue[256];
    DWORD parameterValueBufferSize = 256;
    LPTSTR subKey = new TCHAR[256];
    INT option;
    _tprintf(L"\nSelect initial section:\n1. HKEY_CLASSES_ROOT\n2. HKEY_CURRENT_CONFIG\n\
                3. HKEY_CURRENT_USER\n4. HKEY_LOCAL_MACHINE\n5. HKEY_USERS\nYour option: ");
    _tscanf(L"%i", &option);
    HKEY initialSection = option == 1 ? HKEY_CLASSES_ROOT : option == 2 ? HKEY_CURRENT_CONFIG : option == 3 ? HKEY_CURRENT_USER : option == 4 ? HKEY_LOCAL_MACHINE : option == 5 ? HKEY_USERS : NULL;
    if (initialSection != NULL) {
        _tprintf(L"Input keys path (if you want to print initial section's parametres, enter 'd'): ");
        _tscanf(L"%s", subKey);
        if (lstrcmpW(subKey, L"d") == 0) subKey = (LPTSTR)L"";
        LONG lRes = RegOpenKeyEx(initialSection, subKey, NULL, KEY_QUERY_VALUE, &hKey);
        if (lRes != ERROR_SUCCESS)
        {
            _tprintf(L"Error");
        }
        else
        {
            _tprintf(L"\n");
            _tprintf(L"| %-40s | %-24s | %s\n", L"Name", L"Type", L"Value");
            do
            {
                dwValue = 256;
                subkeyResultCode = RegEnumValue(hKey, dwIndex, parameterName, &dwValue, NULL, &parameterType, parameterValue, &parameterValueBufferSize);
                if (subkeyResultCode == ERROR_SUCCESS)
                {
                    LPTSTR parameterTypeN = new TCHAR[256];
                    switch (parameterType)
                    {
                    case REG_BINARY: parameterTypeN = (LPTSTR)L"REG_BINARY"; break;
                    case REG_DWORD: parameterTypeN = (LPTSTR)L"REG_DWORD"; break;
                    case REG_DWORD_BIG_ENDIAN: parameterTypeN = (LPTSTR)L"REG_DWORD_BIG_ENDIAN"; break;
                    case REG_EXPAND_SZ: parameterTypeN = (LPTSTR)L"REG_EXPAND_SZ"; break;
                    case REG_LINK: parameterTypeN = (LPTSTR)L"REG_LINK"; break;
                    case REG_MULTI_SZ: parameterTypeN = (LPTSTR)L"REG_MULTI_SZ"; break;
                    case REG_NONE: parameterTypeN = (LPTSTR)L"REG_NONE"; break;
                    case REG_RESOURCE_LIST: parameterTypeN = (LPTSTR)L"REG_RESOURCE_LIST"; break;
                    case REG_SZ: parameterTypeN = (LPTSTR)L"REG_SZ"; break;
                    default: parameterTypeN = (LPTSTR)L"UNKOWN "; break;
                    }
                    _tprintf(L" %-40s   ", parameterName);
                    _tprintf(L"%-24s   ", parameterTypeN);
                    _tprintf(L"%s\n", (LPCTSTR)parameterValue);
                }
                else
                {
                    keyName = (LPTSTR)L"Error!";
                }
                dwIndex++;
            } while (subkeyResultCode != ERROR_NO_MORE_ITEMS);
        }
        RegCloseKey(hKey);
    }
}

void SearchString() 
{
    LPTSTR searchValue = new TCHAR[256];
    HKEY    hKey = 0;
    LONG    subkeyResultCode;
    DWORD   dwValue = 256;
    DWORD   dwIndex = 0;
    LPTSTR subkeyName = new TCHAR[256];
    TCHAR subKey[1024];
    INT option;
    _tprintf(L"\nSelect initial section:\n1. HKEY_CLASSES_ROOT\n2. HKEY_CURRENT_CONFIG\n\
                3. HKEY_CURRENT_USER\n4. HKEY_LOCAL_MACHINE\n5. HKEY_USERS\nYour option: ");
    _tscanf(L"%i", &option);
    HKEY initialSection = option == 1 ? HKEY_CLASSES_ROOT : option == 2 ? HKEY_CURRENT_CONFIG : option == 3 ? HKEY_CURRENT_USER : option == 4 ? HKEY_LOCAL_MACHINE : option == 5 ? HKEY_USERS : NULL;
    if (initialSection != NULL) {
        _tprintf(L"Input keys path (if you want to find something here, enter 'd'): ");
        _tscanf(L"%s", subKey);
        if (lstrcmpW(subKey, L"d") == 0) lstrcpy(subKey, L"");
        _tprintf(L"Input search string: ");
        _tscanf(L"%s", searchValue);
        KeyAnalyser(initialSection, subKey, searchValue);
    }
}

void KeyAnalyser(HKEY initialSection, TCHAR keyPath[], LPTSTR searchValue)
 {
    LPTSTR subkeyName = new TCHAR[512];
    LPTSTR subkeyTemp = new TCHAR[512];

    HKEY    hKey = 0;
    LONG    subkeyResultCode;
    LONG    paramsResultCode;
    DWORD   dwValueForKey = 512;
    DWORD   dwIndexForKey = 0;
    DWORD   dwValueForParameter = 512;
    DWORD   dwIndexForParameter = 0;
    TCHAR parameterName[512];
    DWORD parameterType = 0;
    BYTE parameterValue[8192];
    DWORD parameterValueBufferSize = 8192;
    if (lstrlen(keyPath) > 2)
    {
        if (keyPath[0] == L'\\')
        {
            lstrcpyn(keyPath, keyPath + 1, lstrlen(keyPath));
        }
    }
    LONG lRes = RegOpenKeyEx(initialSection, keyPath, NULL, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, &hKey);
    if (lRes != ERROR_SUCCESS)
    {

    }
    else
    {
        do
        {
            dwValueForKey = 256;
            subkeyResultCode = RegEnumKeyEx(hKey, dwIndexForKey, subkeyName, &dwValueForKey, NULL, NULL, NULL, NULL);
            if (subkeyResultCode == ERROR_SUCCESS)
            {

                lstrcpy(subkeyTemp, keyPath);
                lstrcat(subkeyTemp, L"\\");
                lstrcat(subkeyTemp, subkeyName);
                if (_tcsstr(subkeyName, searchValue) != nullptr)
                {
                    _tprintf(L"KEY: ");
                    _tprintf(subkeyTemp);
                    _tprintf(L"\n");
                }
                KeyAnalyser(initialSection, subkeyTemp, searchValue);
            }
            dwIndexForKey++;

            do
            {
                dwValueForParameter = 512;
                paramsResultCode = RegEnumValue(hKey, dwIndexForParameter, parameterName, &dwValueForParameter, NULL, &parameterType, parameterValue, &parameterValueBufferSize);
                if (paramsResultCode == ERROR_SUCCESS)
                {
                    if (_tcsstr(parameterName, searchValue) != nullptr)
                    {
                        _tprintf(L"Parameter name: ");
                        _tprintf(parameterName);
                        _tprintf(L" Path: %s", keyPath);
                        _tprintf(L"\n");
                    }
                    if (_tcsstr((TCHAR *)parameterValue, searchValue) != nullptr)
                    {
                        _tprintf(L"Parameter value: ");
                        _tprintf((TCHAR *)parameterValue);
                        _tprintf(L" Path: %s", keyPath);
                        _tprintf(L"\n");
                    }
                }
                dwIndexForParameter++;
            } while (paramsResultCode != ERROR_NO_MORE_ITEMS);
        } while (subkeyResultCode != ERROR_NO_MORE_ITEMS);
    }
    RegCloseKey(hKey);
}

void SaveKey()
{
    HKEY    hKey = 0;
    TCHAR subKey[256];
    TCHAR fileName[256];
    INT option;
    _tprintf(L"\nSelect initial section:\n1. HKEY_CLASSES_ROOT\n2. HKEY_CURRENT_CONFIG\n\
                3. HKEY_CURRENT_USER\n4. HKEY_LOCAL_MACHINE\n5. HKEY_USERS\nYour option: ");
    _tscanf(L"%i", &option);
    HKEY initialSection = option == 1 ? HKEY_CLASSES_ROOT : option == 2 ? HKEY_CURRENT_CONFIG : option == 3 ? HKEY_CURRENT_USER : option == 4 ? HKEY_LOCAL_MACHINE : option == 5 ? HKEY_USERS : NULL;
    if (initialSection != NULL) {
        _tprintf(L"Input keys path (if you want to save initial section, enter 'd'): ");
        _tscanf(L"%s", subKey);
        if (lstrcmpW(subKey, L"d") == 0) lstrcpy(subKey, L"");
        LONG lRes = RegOpenKeyEx(initialSection, subKey, NULL, KEY_ALL_ACCESS, &hKey);
        if (lRes == ERROR_SUCCESS)
        {
            _tprintf(L"Input file name (format - path\\to\\file\\filename.file_extension): ");
            _tscanf(L"%s", fileName);
            DWORD errcode;

            LSTATUS response = RegSaveKeyEx(hKey, fileName, NULL, REG_NO_COMPRESSION);
            errcode = GetLastError();
            if (response == ERROR_SUCCESS)
            {
                _tprintf(L"Successfully!\n");
            }
            if (response == ERROR_INVALID_PARAMETER)
            {
                _tprintf(L"Error invalid parameter");
            }
            else {
                _tprintf(L"Unsuccessfully!\n");
                LPTSTR lpMsg = NULL;

                FormatMessage(
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_ALLOCATE_BUFFER,
                    NULL,
                    response,
                    MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                    (LPWSTR)&lpMsg,
                    0,
                    NULL);
                _tprintf(L"Error message - %s\n", lpMsg);
                LocalFree(lpMsg);
            }
        }
        _tprintf(L"Error opening key!\n");
    }
}
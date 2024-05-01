#include "fileinformation.h"

void TraverseSubDirectories(LPTSTR currPath) {
    WIN32_FIND_DATA FindFileData;
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    hFind = FindFirstFile(currPath, &FindFileData);
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                printf("%s\n", FindFileData.cFileName);
            }
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
                printf("%s\n", FindFileData.cFileName);
            }
        } while (FindNextFile(hFind, &FindFileData));

        FindClose(hFind);
    }

    printf("\n\n");
}

void GetDirectories(LPTSTR lpFolder) {
    TCHAR szFullPattern[MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    HANDLE hFindFile;
    int i = 0;
    int count = 0;

    PathCombine(szFullPattern, lpFolder, TEXT("*"));
    hFindFile = FindFirstFile(szFullPattern, &FindFileData);

    if (hFindFile != INVALID_HANDLE_VALUE) {
        do {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  {
                StringCchCopy(dirBlock[i].dirName, BUFSIZE, FindFileData.cFileName);
                PathCombine(dirBlock[i].dirName, lpFolder, FindFileData.cFileName);
                StringCchCat(dirBlock[i].dirName, MAX_PATH, TEXT("\\*"));
                dirBlock[i].dwDirAttribute = FindFileData.dwFileAttributes;
                // printf("%s %d\n", dirBlock[i].dirName, dirBlock[i].dwDirAttribute);
            }

            i++;
            count++;
        } while (FindNextFile(hFindFile, &FindFileData));

        FindClose(hFindFile);
    }

    for (i = 0; i < count; i++) {
        if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY)
            TraverseSubDirectories(dirBlock[i].dirName);
    }
}

// Function made by Steven Engelhardt
int GetDWORDNumberFormat(LCID locale, DWORD dwFlags, DWORD value, 
    const NUMBERFMT* lpFormat, LPTSTR lpNumberStr, int cchNumber) {
    TCHAR szBuf[317];
    HRESULT hr = StringCchPrintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), 
        TEXT("%d"), value);

    if (hr != S_OK) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }

    return GetNumberFormat(locale, dwFlags, szBuf, lpFormat, lpNumberStr, cchNumber);
}

int GetFileOwner(LPTSTR currentDir) {
    DWORD dwRtnCode = 0;
    PSID pSidOwner = NULL;
    BOOL bRtnBool = TRUE;
    LPTSTR AcctName = NULL;
    LPTSTR DomainName = NULL;
    DWORD dwAcctname = 1;
    DWORD dwDomainName = 1;
    SID_NAME_USE eUse = SidTypeUnknown;
    WIN32_FIND_DATA fileData;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    HGLOBAL mem;
    PSECURITY_DESCRIPTOR pSD = NULL;
    size_t lengthOfArg = sizeof(currentDir);
    int i = 0;

    if (lengthOfArg > (MAX_PATH - 3)) {
        _tprintf(TEXT("\nDirectory path is too long\n"));
        return 0;
    }

    StringCchCat(currentDir, BUFSIZE, "\\*");
    hFind = FindFirstFile(currentDir, &fileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        _tprintf(TEXT("Could not find the first file in GetFileOwner (%d)\n"), 
            GetLastError());
        return 0;
    }

    do {
        AcctName = (LPTSTR)GlobalAlloc(GMEM_ZEROINIT, 0);
        DomainName = (LPTSTR)GlobalAlloc(GMEM_ZEROINIT, 0);

        if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
                dwDomainName = 0;
                dwAcctname = 0;

                if (pSidOwner != NULL) {
                    pSidOwner = NULL;
                }

                if (AcctName && DomainName) {
                    AcctName = NULL;
                    DomainName = NULL;
                }

                hFile = CreateFile(dirBlock[i].dirName, FILE_GENERIC_READ, FILE_SHARE_READ,
                    NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

                if (hFile == INVALID_HANDLE_VALUE)
                    continue;

                dwRtnCode = GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION,
                    &pSidOwner, NULL, NULL, NULL, &pSD);
                LookupAccountSid(NULL, pSidOwner, AcctName, 
                    (LPDWORD)&dwAcctname, DomainName, (LPDWORD)&dwDomainName, &eUse);

                AcctName = (LPTSTR)GlobalAlloc(GPTR, dwAcctname * sizeof(wchar_t));
                DomainName = (LPTSTR)GlobalAlloc(GPTR, dwDomainName * sizeof(wchar_t));
                bRtnBool = LookupAccountSid(NULL, pSidOwner, AcctName, 
                    (LPDWORD)&dwAcctname, DomainName, (LPDWORD)&dwDomainName, &eUse);

                if (bRtnBool == TRUE) {
                    StringCchCopy(dirBlock[i].accountOwner, BUFSIZE, AcctName);
                    StringCchCopy(dirBlock[i].ownerMachine, BUFSIZE, DomainName);
                }
            }

            else {
                dwDomainName = 0;
                dwAcctname = 0;

                if (pSidOwner != NULL) {
                    pSidOwner = NULL;
                }

                if (AcctName && DomainName) {
                    AcctName = NULL;
                    DomainName = NULL;
                }

                hFile = CreateFile(dirBlock[i].dirName, FILE_GENERIC_READ, FILE_SHARE_READ,
                    NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

                if (hFile == INVALID_HANDLE_VALUE)
                    continue;

                dwRtnCode = GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION,
                    &pSidOwner, NULL, NULL, NULL, &pSD);
                LookupAccountSid(NULL, pSidOwner, AcctName, 
                    (LPDWORD)&dwAcctname, DomainName, (LPDWORD)&dwDomainName, &eUse);

                AcctName = (LPTSTR)GlobalAlloc(GPTR, dwAcctname * sizeof(wchar_t));                
                DomainName = (LPTSTR)GlobalAlloc(GPTR, dwDomainName * sizeof(wchar_t));
                bRtnBool = LookupAccountSid(NULL, pSidOwner, AcctName, 
                    (LPDWORD)&dwAcctname, DomainName, (LPDWORD)&dwDomainName, &eUse);

                if (bRtnBool == TRUE) {
                    StringCchCopy(dirBlock[i].accountOwner, BUFSIZE, AcctName);
                    StringCchCopy(dirBlock[i].ownerMachine, BUFSIZE, DomainName);
                }
            }
        }

        if (fileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
            dwDomainName = 0;
            dwAcctname = 0;

            if (pSidOwner != NULL) {
                pSidOwner = NULL;
            }

            if (AcctName && DomainName) {
                AcctName = NULL;
                DomainName = NULL;
            }

            hFile = CreateFile(block[i].fileName, FILE_GENERIC_READ, FILE_SHARE_READ,
                NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

            if (hFile == INVALID_HANDLE_VALUE)
                continue;

            dwRtnCode = GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION,
                &pSidOwner, NULL, NULL, NULL, &pSD);

            LookupAccountSid(NULL, pSidOwner, AcctName, 
                (LPDWORD)&dwAcctname, DomainName, (LPDWORD)&dwDomainName, &eUse);

            AcctName = (LPTSTR)GlobalAlloc(GMEM_FIXED, dwAcctname * sizeof(wchar_t));
            DomainName = (LPTSTR)GlobalAlloc(GMEM_FIXED, dwDomainName * sizeof(wchar_t));

            bRtnBool = LookupAccountSid(NULL, pSidOwner, AcctName, 
                (LPDWORD)&dwAcctname, DomainName, (LPDWORD)&dwDomainName, &eUse);

            if (bRtnBool == TRUE) {
                StringCchCopy(block[i].accountOwner, BUFSIZE, AcctName);
                StringCchCopy(block[i].ownerMachine, BUFSIZE, DomainName);
            }
        }

        i++;
    } while (FindNextFile(hFind, &fileData) != 0);

    FindClose(hFind);
    CloseHandle(hFile);
    return 1;
}

int GetTimeField(LPTSTR currentDir, LPTSTR option) {
    WIN32_FIND_DATA fileData;
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    size_t lengthOfArg = sizeof(currentDir);
    int i = 0;
    int files = 0;

    if (lengthOfArg > (MAX_PATH - 3)) {
        _tprintf(TEXT("\nDirectory path is too long\n"));
        return 0;
    }

    StringCchCat(currentDir, BUFSIZE, "\\*");
    hFind = FindFirstFile(currentDir, &fileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        _tprintf(TEXT("Could not find the first file in GetTimeField (%d)\n"), 
            GetLastError());
        return 0;
    }

    do {
        hFile = CreateFile(fileData.cFileName, GENERIC_READ, FILE_SHARE_READ, 
            NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            continue;
        }

        if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
            return 0;

        if (lstrcmpA(option, "/T:C") == 0) {
            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (fileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
                    FileTimeToSystemTime(&ftCreate, &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    dirBlock[i].dwDirAttribute = GetFileAttributes(fileData.cFileName);
                    StringCchCopy(dirBlock[i].dirName, BUFSIZE, fileData.cFileName);
                    dirBlock[i].wYear = stLocal.wYear;
                    dirBlock[i].wMonth = stLocal.wMonth;
                    dirBlock[i].wDay = stLocal.wDay;
                    dirBlock[i].wHour = stLocal.wHour;
                    dirBlock[i].wMinute = stLocal.wMinute;
                }

                else {
                    FileTimeToSystemTime(&ftCreate, &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);    
                    dirBlock[i].dwDirAttribute = GetFileAttributes(fileData.cFileName);
                    StringCchCopy(dirBlock[i].dirName, BUFSIZE, fileData.cFileName);
                    dirBlock[i].wYear = stLocal.wYear;
                    dirBlock[i].wMonth = stLocal.wMonth;
                    dirBlock[i].wDay = stLocal.wDay;
                    dirBlock[i].wHour = stLocal.wHour;
                    dirBlock[i].wMinute = stLocal.wMinute;
                }
            }

            else {
                FileTimeToSystemTime(&ftCreate, &stUTC);
                SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);                
                block[i].dwFileAttribute = GetFileAttributes(fileData.cFileName);
                StringCchCopy(block[i].fileName, BUFSIZE, fileData.cFileName);
                block[i].wYear = stLocal.wYear;
                block[i].wMonth = stLocal.wMonth;
                block[i].wDay = stLocal.wDay;
                block[i].wHour = stLocal.wHour;
                block[i].wMinute = stLocal.wMinute;
            }
        }

        if (lstrcmpA(option, "/T:A") == 0) {
            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (fileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
                    FileTimeToSystemTime(&ftAccess, &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    dirBlock[i].dwDirAttribute = GetFileAttributes(fileData.cFileName);
                    StringCchCopy(dirBlock[i].dirName, BUFSIZE, fileData.cFileName);
                    dirBlock[i].wYear = stLocal.wYear;
                    dirBlock[i].wMonth = stLocal.wMonth;
                    dirBlock[i].wDay = stLocal.wDay;
                    dirBlock[i].wHour = stLocal.wHour;
                    dirBlock[i].wMinute = stLocal.wMinute;
                }

                else {
                    FileTimeToSystemTime(&ftAccess, &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    dirBlock[i].dwDirAttribute = GetFileAttributes(fileData.cFileName);
                    StringCchCopy(dirBlock[i].dirName, BUFSIZE, fileData.cFileName);
                    dirBlock[i].wYear = stLocal.wYear;
                    dirBlock[i].wMonth = stLocal.wMonth;
                    dirBlock[i].wDay = stLocal.wDay;
                    dirBlock[i].wHour = stLocal.wHour;
                    dirBlock[i].wMinute = stLocal.wMinute;
                }
            }

            else {
                FileTimeToSystemTime(&ftAccess, &stUTC);
                SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);                
                block[i].dwFileAttribute = GetFileAttributes(fileData.cFileName);
                StringCchCopy(block[i].fileName, BUFSIZE, fileData.cFileName);
                block[i].wYear = stLocal.wYear;
                block[i].wMonth = stLocal.wMonth;
                block[i].wDay = stLocal.wDay;
                block[i].wHour = stLocal.wHour;
                block[i].wMinute = stLocal.wMinute;
            }
        }

        if (lstrcmpA(option, "/T:W") == 0) {
            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (fileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
                    FileTimeToSystemTime(&ftWrite, &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    dirBlock[i].dwDirAttribute = GetFileAttributes(fileData.cFileName);
                    StringCchCopy(dirBlock[i].dirName, BUFSIZE, fileData.cFileName);
                    dirBlock[i].wYear = stLocal.wYear;
                    dirBlock[i].wMonth = stLocal.wMonth;
                    dirBlock[i].wDay = stLocal.wDay;
                    dirBlock[i].wHour = stLocal.wHour;
                    dirBlock[i].wMinute = stLocal.wMinute;
                }

                else {
                    FileTimeToSystemTime(&ftWrite, &stUTC);
                    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                    dirBlock[i].dwDirAttribute = GetFileAttributes(fileData.cFileName);
                    StringCchCopy(dirBlock[i].dirName, BUFSIZE, fileData.cFileName);
                    dirBlock[i].wYear = stLocal.wYear;
                    dirBlock[i].wMonth = stLocal.wMonth;
                    dirBlock[i].wDay = stLocal.wDay;
                    dirBlock[i].wHour = stLocal.wHour;
                    dirBlock[i].wMinute = stLocal.wMinute;
                }
            }

            else {
                FileTimeToSystemTime(&ftWrite, &stUTC);
                SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
                block[i].dwFileAttribute = GetFileAttributes(fileData.cFileName);
                StringCchCopy(block[i].fileName, BUFSIZE, fileData.cFileName);
                block[i].wYear = stLocal.wYear;
                block[i].wMonth = stLocal.wMonth;
                block[i].wDay = stLocal.wDay;
                block[i].wHour = stLocal.wHour;
                block[i].wMinute = stLocal.wMinute;
            }
        }
        
        i++;
    } while (FindNextFile(hFind, &fileData) != 0);

    FindClose(hFind);
    CloseHandle(hFile);
    return 1;
}

int SetThousandSeparator(int files) {
    int i = 0;
    int ret = 0;
    TCHAR buf[80];

    for (i = 0; i < files; i++) {
        ret = GetDWORDNumberFormat(LOCALE_USER_DEFAULT, 0, block[i].dwFileSize,
            NULL, buf, sizeof(buf) / sizeof(buf[0]));
        StringCchPrintf(block[i].fileThousandSpr, BUFSIZE, TEXT("%s"), buf);
    }

    return ret;
}
#include "fileinformation.h"

FileInformation block[BUFSIZE];
DirInformation dirBlock[BUFSIZE];

FileInformation InitFileBlock() {
    int i;
    static FileInformation block[BUFSIZE];

    for (i = 0; i < sizeof(block) / sizeof(block[0]); i++) {
        StringCchCopy(block[i].fileName, BUFSIZE, "-");
        StringCchCopy(block[i].fileExt, BUFSIZE, "-");
        StringCchCopy(block[i].accountOwner, BUFSIZE, "-");
        StringCchCopy(block[i].ownerMachine, BUFSIZE, "-");
        StringCchCopy(block[i].ownerDomain, BUFSIZE, "-");
        block[i].count = 0;
        block[i].dwFileSize = 0;
        block[i].dwFileAttribute = 0;
        block[i].wYear = 0;
        block[i].wMonth = 0;
        block[i].wDay = 0;
        block[i].wHour = 0;
        block[i].wMinute = 0;
        block[i].wSecond = 0;
        block[i].wMilliSeconds = 0;
    }

    return *block;
}

DirInformation InitDirBlock() {
    int i;
    static DirInformation dirBlock[BUFSIZE];

    for (i = 0; i < sizeof(dirBlock) / sizeof(dirBlock[0]); i++) {
        StringCchCopy(dirBlock[i].dirName, BUFSIZE, "-");
        StringCchCopy(dirBlock[i].accountOwner, BUFSIZE, "-");
        StringCchCopy(dirBlock[i].ownerMachine, BUFSIZE, "-");
        StringCchCopy(dirBlock[i].ownerDomain, BUFSIZE, "-");
        dirBlock[i].count = 0;
        dirBlock[i].dwDirAttribute = 0;
        dirBlock[i].wYear = 0;
        dirBlock[i].wMonth = 0;
        dirBlock[i].wDay = 0;
        dirBlock[i].wHour = 0;
        dirBlock[i].wMinute = 0;
        dirBlock[i].wSecond = 0;
        dirBlock[i].wMilliSeconds = 0;
    }

    return *dirBlock;
}

int DirListing(LPTSTR lpszPath) {
    WIN32_FIND_DATA ffd;
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    LARGE_INTEGER fileSize;
    TCHAR szDir[MAX_PATH];
    TCHAR szBuf[MAX_PATH];
    size_t lengthOfArg = sizeof(lpszPath);
    HANDLE hFind = INVALID_HANDLE_VALUE;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    int i = 0, j = 0, k = 0;
    DWORD dwError = 0;
    DWORD dwRet = 0;

    if (lengthOfArg > (MAX_PATH - 3)) {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return 0;
    }

    StringCchCopy(szDir, MAX_PATH, lpszPath);
    StringCchCat(szDir, MAX_PATH, "\\*");
    hFind = FindFirstFile(szDir, &ffd);

    if (hFind == INVALID_HANDLE_VALUE) {
        _tprintf(TEXT("Could not find the first file in DirListing (%d)\n"), 
            GetLastError());
        return 0;

    }

    // List all the files in the directory with some basic information
    do {
        hFile = CreateFile(ffd.cFileName, GENERIC_READ, FILE_SHARE_READ,
            NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
            continue;

        if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
            return FALSE;

        FileTimeToSystemTime(&ftWrite, &stUTC);
        SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
                dirBlock[i].dwDirAttribute = GetFileAttributes(ffd.cFileName);
                StringCchCopy(dirBlock[i].dirName, BUFSIZE, ffd.cFileName);
                StringCchCopy(dirBlock[i].alternateName, BUFSIZE, ffd.cAlternateFileName);
                dirBlock[i].wYear = stLocal.wYear;
                dirBlock[i].wMonth = stLocal.wMonth;
                dirBlock[i].wDay = stLocal.wDay;
                dirBlock[i].wHour = stLocal.wHour;
                dirBlock[i].wMinute = stLocal.wMinute;
                dirBlock[i].count = j;
                j++;
            }

            else {
                dirBlock[i].dwDirAttribute = GetFileAttributes(ffd.cFileName);
                dirBlock[i].wYear = stLocal.wYear;
                dirBlock[i].wMonth = stLocal.wMonth;
                dirBlock[i].wDay = stLocal.wDay;
                dirBlock[i].wHour = stLocal.wHour;
                dirBlock[i].wMinute = stLocal.wMinute;
                dirBlock[i].count = j;
                StringCchCopy(dirBlock[i].dirName, BUFSIZE, ffd.cFileName);
                StringCchCopy(dirBlock[i].alternateName, BUFSIZE, ffd.cAlternateFileName);
                j++;
            }
        }

        else {
            fileSize.LowPart = ffd.nFileSizeLow;
            fileSize.HighPart = ffd.nFileSizeHigh;
            StringCchCopy(block[i].fileName, BUFSIZE, ffd.cFileName);
            StringCchCopy(block[i].alternateName, BUFSIZE, ffd.cAlternateFileName);
            if (PathFindExtensionA(block[i].fileName) != NULL)
                StringCchCopy(block[i].fileExt, BUFSIZE, PathFindExtensionA(block[i].fileName));

            block[i].dwFileAttribute = GetFileAttributes(ffd.cFileName);
            block[i].dwFileSize = ffd.nFileSizeLow;
            block[i].wYear = stLocal.wYear;
            block[i].wMonth = stLocal.wMonth;
            block[i].wDay = stLocal.wDay;
            block[i].wHour = stLocal.wHour;
            block[i].wMinute = stLocal.wMinute;
            block[i].count = k;
            k++;
        }

        i++;
    } while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);
    CloseHandle(hFile);
    return i;
}

void ShowOptions() {
    _tprintf(TEXT("DirTraversion (full pathname) / (relative pathname)\n\n[/A:Attributes][/O:Sortorder][/T:Timefield]\n"));
    _tprintf(TEXT("[/B][/C][/D][/P][/Q][/S][/W][/X]\n\n"));
    _tprintf(TEXT("/A: Display the file attrributes.\nAttributes\t"));
    _tprintf(TEXT("D Directory\tR Read Only\n\t\tH Hidden\tA Archive ready files\n\t\tS System files\t"
                "I Not content indexed files\n\t\tL Reparse Points\n"));
    _tprintf(TEXT("\n/O: Display files in a certain sort order.\nSortorder\t"));
    _tprintf(TEXT("D By date (oldest first)\tS By size (smallest first)\n\t\tG Group directories first\t"
        "E By extension (alphabetic)\n\t\tN By name (alphabetic)\n"));
    _tprintf(TEXT("\n/T: Controls which time field is displayed or used for sorting.\nTimefield\t"));
    _tprintf(TEXT("C Creation\tA Last Access\tW Last Written\n\n"));
    _tprintf(TEXT("/B: Display the files in bare format (no heading information or summary).\n"));
    _tprintf(TEXT("/C: Display the thousand separator in file sizes.\n"));
    _tprintf(TEXT("/D: Display files in wide format but directories are shown first.\n"));
    _tprintf(TEXT("/P: Pauses the traversion display.\n"));
    _tprintf(TEXT("/Q: Display the file owner.\n"));
    _tprintf(TEXT("/S: Display the files in the current directory and the subdirectories.\n"));
    _tprintf(TEXT("/W: Display files in wide format.\n"));
    _tprintf(TEXT("/X: Display the short name of non-8dot3 file names.\n"));
    _tprintf(TEXT("/?: Display this help screen.\n\n"));
}


int main(int argc, TCHAR **argv) {
    TCHAR dirLocation[BUFSIZE];
    TCHAR newDirLocation[BUFSIZE];
    DWORD dwRet;
    int numOfFiles = 0;
    int dispLang = 0;
    unsigned int cp;

    InitFileBlock();
    InitDirBlock();
    cp = GetConsoleOutputCP();
    SetConsoleOutputCP(cp);
    dispLang = GetDisplayLang();
    
    if (argc < 2) {
        dwRet = GetCurrentDirectory(BUFSIZE, dirLocation);

        if (dwRet == 0) {
            _tprintf(TEXT("GetCurrentDirectory failed (%d)\n"), GetLastError());
            return;
        }

        if (dwRet > BUFSIZE) {
            _tprintf(TEXT("Buffer too small, need %d characters\n"), dwRet);
            return;
        }

        numOfFiles = DirListing(dirLocation);
        
        switch (dispLang) {
            case 1033:
                PrintFileInfo_enUS(dirLocation, numOfFiles);
                break;

            case 1046:
                PrintFileInfo_ptBR(dirLocation, numOfFiles);
                break;
        }
        // printf("CP -> %ld\n", cp);
        return;
    }

    if (PathFileExistsA(argv[1])) {
        StringCchCopy(dirLocation, MAX_PATH, argv[1]);
        SetCurrentDirectory(dirLocation);
        GetCurrentDirectory(MAX_PATH, dirLocation);
        numOfFiles = DirListing(dirLocation);

        if (argv[2]) {
            ProcessOptions(dirLocation, argv[2], numOfFiles, dispLang);
        }

        else {
            switch (dispLang) {
                case 1033:
                    PrintFileInfo_enUS(dirLocation, numOfFiles);
                    break;

                case 1046:
                    PrintFileInfo_ptBR(dirLocation, numOfFiles);
                    break;
            }
        }
    }

    if (lstrcmpA(argv[1], "/?") == 0) {
        ShowOptions();
    }

    return 0;
}
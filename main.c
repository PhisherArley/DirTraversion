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

int main(int argc, TCHAR **argv) {
    TCHAR dirLocation[BUFSIZE];
    TCHAR newDirLocation[BUFSIZE];
    DWORD dwRet;
    int numOfFiles = 0;
    unsigned int cp;

    InitFileBlock();
    InitDirBlock();
    cp = GetConsoleOutputCP();
    SetConsoleOutputCP(cp);

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
        PrintFileInfo(dirLocation, numOfFiles);
        // printf("CP -> %ld\n", cp);
        return;
    }

    if (PathFileExistsA(argv[1])) {
        StringCchCopy(dirLocation, MAX_PATH, argv[1]);
        SetCurrentDirectory(dirLocation);
        GetCurrentDirectory(MAX_PATH, dirLocation);
        numOfFiles = DirListing(dirLocation);

        if (argv[2]) {
            ProcessOptions(dirLocation, argv[2], numOfFiles);
        }

        else
            PrintFileInfo(dirLocation, numOfFiles);
    }

    if (lstrcmpA(argv[1], "/?") == 0) {
        ShowOptions();
    }

    return 0;
}
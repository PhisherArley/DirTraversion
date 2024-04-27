#ifndef _FILEINFORMATION_H
#define _FILEINFORMATION_H
#endif

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <strsafe.h>
#include <shlwapi.h>
#include <string.h>
#include <accctrl.h>
#include <aclapi.h>
#include <fileapi.h>
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "advapi32.lib")

#define BUFSIZE MAX_PATH

void GetSubdirectoriesTime(LPTSTR File);

typedef struct FileInformation {
    TCHAR fileName[BUFSIZE];
    TCHAR alternateName[BUFSIZE];
    TCHAR fileExt[20];
    TCHAR fileThousandSpr[BUFSIZE];
    TCHAR accountOwner[BUFSIZE];
    TCHAR ownerMachine[BUFSIZE];
    TCHAR ownerDomain[BUFSIZE];
    int count;
    DWORD dwFileSize;
    DWORD dwFileAttribute;
    WORD wYear;
    WORD wMonth;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliSeconds;
} FileInformation;

typedef struct DirInformation {
    TCHAR dirName[BUFSIZE];
    TCHAR alternateName[BUFSIZE];
    TCHAR accountOwner[BUFSIZE];
    TCHAR ownerMachine[BUFSIZE];
    TCHAR ownerDomain[BUFSIZE];
    int count;
    DWORD dwDirAttribute;
    WORD wYear;
    WORD wMonth;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliSeconds;
} DirInformation;

extern FileInformation block[BUFSIZE];
extern DirInformation dirBlock[BUFSIZE];

extern FileInformation InitFileBlock();
extern DirInformation InitDirBlock();
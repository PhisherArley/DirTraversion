#include "fileinformation.h"

void PrintDirFirst_ptBR(LPTSTR currPath, int files) {
    int i;
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    for (i = 0; i < files; i++) {
        if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
            if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
                    _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<SYMLINKD>\t%s\n"), 
                        dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                        dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
            }

            else {
                _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%s\n"), 
                    dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                    dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
            }
        }
    }

    for (i = 0; i < files; i++) {
        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
            continue;

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
                _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%d\t%s\n"), 
                    block[i].wDay, block[i].wMonth, block[i].wYear, 
                    block[i].wHour, block[i].wMinute, block[i].dwFileSize,
                    block[i].fileName);
        }
    }
}

void PrintFileInfo_ptBR(LPTSTR currPath, int files) {
    int i;
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    if (currPath) {
        for (i = 0; i < files; i++) {
            if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
                if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
                    _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<SYMLINKD>\t%s\n"), 
                        dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                        dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
                }

                else {
                    _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%s\n"), 
                        dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                        dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
                }
            }

            if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
                continue;

            if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
                _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%d\t%s\n"), 
                    block[i].wDay, block[i].wMonth, block[i].wYear, 
                    block[i].wHour, block[i].wMinute, block[i].dwFileSize,
                    block[i].fileName);
            }
        }
    }
}

void PrintThousandFormat_ptBR(LPTSTR currPath, int files, LPTSTR option) {
    int i;
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    if (lstrcmpA(option, "/C") == 0) {
        for (i = 0; i < files; i++) {
            if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
                if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
                    _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<SYMLINKD>\t%s\n"), 
                        dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                        dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
                }

                else {
                    _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%s\n"), 
                        dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                        dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
                }
            }

            RemoveComma(block[i].fileThousandSpr);

            if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
                continue;

            if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
                _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%s\t%s\n"), 
                    block[i].wDay, block[i].wMonth, block[i].wYear, 
                    block[i].wHour, block[i].wMinute, block[i].fileThousandSpr,
                    block[i].fileName);
            }
        }
    }
}

void PrintSortedFile_ptBR(LPTSTR currPath, int files) {
    int i, j;
    FileInformation temp[BUFSIZE];
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    for (i = 0; i < files; i++) {
        for (j = i + 1; j < files; j++) {
            if (lstrcmpA(block[i].fileName, block[j].fileName) > 0 && 
                !(block[i].dwFileAttribute & FILE_ATTRIBUTE_DIRECTORY)) {
                temp[i] = block[i];
                block[i] = block[j];
                block[j] = temp[i];
            }
        }
    }

    for (i = 0; i < files; i++) {
        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
            continue;

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
            _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%s\t%s\n"), 
                block[i].wDay, block[i].wMonth, block[i].wYear, 
                block[i].wHour, block[i].wMinute, block[i].fileThousandSpr,
                block[i].fileName);
        }
    }
}

void PrintShortName_ptBR(LPTSTR currPath, int files) {
    int i;
    int length = 0;
    TCHAR *buffer = NULL;
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    for (i = 0; i < files; i++) {
        if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
            if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
                if (dirBlock[i].dirName != NULL) {
                    _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<SYMLINKD>\t%*s\t%s\n"), 
                        dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                        dirBlock[i].wHour, dirBlock[i].wMinute, 15,
                        dirBlock[i].alternateName, dirBlock[i].dirName);
                }
            }

            else {
                if (dirBlock[i].dirName != NULL) {
                    _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%*s\t%s\n"), 
                        dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                        dirBlock[i].wHour, dirBlock[i].wMinute, 15,
                        dirBlock[i].alternateName, dirBlock[i].dirName);
                }
            }
        }

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
            continue;

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
            if (block[i].fileName != NULL) {
                _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%d\t%*s\t%s\n"), 
                    block[i].wDay, block[i].wMonth, block[i].wYear, 
                    block[i].wHour, block[i].wMinute, block[i].dwFileSize,
                    15, block[i].alternateName, block[i].fileName);
            }
        }
    }
}

void PrintFileOwner_ptBR(LPTSTR currPath, int files) {
    int i;
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    for (i = 0; i < files; i++) {
        if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
            if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
                _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<SYMLINKD>\t%s/%s   %s\n"), 
                    dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                    dirBlock[i].wHour, dirBlock[i].wMinute, 
                    dirBlock[i].accountOwner, dirBlock[i].ownerMachine,
                    dirBlock[i].dirName);

            }
            else {
                _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%s/%s   %s\n"), 
                    dirBlock[i].wDay, dirBlock[i].wMonth, dirBlock[i].wYear,
                    dirBlock[i].wHour, dirBlock[i].wMinute, 
                    dirBlock[i].accountOwner, dirBlock[i].ownerMachine, 
                    dirBlock[i].dirName);
            }
        }

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
            continue;

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
            // if (strlen(block[i].fileName) <= 16)
            _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%d\t%s/%s   %s\n"), 
                block[i].wDay, block[i].wMonth, block[i].wYear, 
                block[i].wHour, block[i].wMinute, block[i].dwFileSize,
                block[i].accountOwner, block[i].ownerMachine,
                block[i].fileName);
        }
    }
}
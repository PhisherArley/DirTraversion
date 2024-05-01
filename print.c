#include "fileinformation.h"

void PrintFileExt(LPTSTR currPath, int files) {
    int i, j;
    FileInformation temp[BUFSIZE];
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    for (i = 0; i < files; i++) {
        for (j = i + 1; j < files; j++) {
            if (block[j].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
                if (lstrcmpA(block[i].fileExt, block[j].fileExt) > 0 && 
                    !(block[i].dwFileAttribute & FILE_ATTRIBUTE_DIRECTORY)) {
                    temp[i] = block[i];
                    block[i] = block[j];
                    block[j] = temp[i];
                }
            }
        }
    }

    for (i = 0; i < files; i++) {
        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
            continue;

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
            _tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%s\n"), 
                block[i].wMonth, block[i].wDay, block[i].wYear, 
                block[i].wHour, block[i].wMinute, block[i].fileName);
        }
    }
}

void PrintBareFile(LPTSTR currPath, int files) {
    int i;
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    for (i = 0; i < files; i++) {
        if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
            if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
                _tprintf(TEXT("%s\n"), dirBlock[i].dirName);
            }

            else
                _tprintf(TEXT("%s\n"), dirBlock[i].dirName);
        }

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
            continue;

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE)
            _tprintf(TEXT("%s\n"), block[i].fileName);
    }
}

void PrintFileInWideFormat(LPTSTR currPath, int files) {
    int i, j;
    int dirTotal = 0, fileTotal = 0;
    int columns = 0, count = 0;
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    for (i = 0; i <= files; i++) {
        if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY)
            dirTotal = dirBlock[i].count;

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE)
            fileTotal = block[i].count;
    }

    columns = (dirTotal / (fileTotal + 1));

    if (columns <= 5) {
        columns = 4;
    }

    for (i = 0; i <= files; i++) {
        if (count == columns) {
            printf("\n");
            count = 0;
        }

        if (count < columns) {
            if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
                if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT)
                    printf("[%s]  ", dirBlock[i].dirName);
                else
                    printf("[%s]\t", dirBlock[i].dirName);
            }

            if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
                continue;

            if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
                if (strlen(block[i].fileName) <= 16)
                    printf("%s   ", block[i].fileName);
                else
                    printf("%s\t", block[i].fileName);
            }
        }
        count++;
    }
}

void PrintFileInColumn(LPTSTR currPath, int files) {
    int i = 0;
    int dirTotal = 0, fileTotal = 0;
    int columns = 0;
    int count = 0;
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    for (i = 0; i <= files; i++) {
        if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY)
            dirTotal = dirBlock[i].count + 1;

        if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE)
            fileTotal = block[i].count + 1;
    }

    columns = (dirTotal / (fileTotal + 1));

    if (columns <= 5)
        columns = 4;


    for (i = 0; i <= files; i++) {
        if (count == columns) {
            printf("\n");
            count = 0;
        }

        if (count < columns) {
            if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
                if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
                    _tprintf(TEXT("[%s]\t"), dirBlock[i].dirName);
                }

                else {
                    _tprintf(TEXT("[%s]\t"), dirBlock[i].dirName);
                }
            }
        }

        count++;
    }

    count = 0;
    printf("\n\n");
    
    for (i = 2; i < files; i++) {
        if (count == columns) {
            printf("\n");
            count = 0;
        }

        if (count < columns) {
            if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
                _tprintf(TEXT("%s\t"), block[i].fileName);
            }
        }

        count++;
    }

    printf("\n\n");
}

void PauseFileOutput(LPTSTR currPath, int files, int langID) {
    int c = 0;
    int i = 0;
    
    if (files > 20) {
        for (i = 0; i < files; i++) {
            int result;
            
            if (langID == 1033)
                PrintFileInfo_enUS(currPath, i);

            if (langID == 1046)
                PrintFileInfo_ptBR(currPath, files);

            if(getchar() != 27) {
                printf("Press enter to continue...\n");
            }
        }
    }

    if (langID == 1033)
        PrintFileInfo_enUS(currPath, files);

    if (langID == 1046)
            PrintFileInfo_ptBR(currPath, files);
    
}
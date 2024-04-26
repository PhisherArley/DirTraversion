#include "fileinformation.h"

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

void PrintDirFirst(LPTSTR currPath, int files) {
	int i;
	strtok(currPath, "*");
	printf("\n%s\n\n", currPath);

	for (i = 0; i < files; i++) {
		if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
			if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
    				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<SYMLINKD>\t%s\n"), 
    					dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
    					dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
			}

			else {
   				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%s\n"), 
   					dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
   					dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
			}
		}
	}

	for (i = 0; i < files; i++) {
		if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
			continue;

		if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
   				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%d\t%s\n"), 
   					block[i].wMonth, block[i].wDay, block[i].wYear, 
   					block[i].wHour, block[i].wMinute, block[i].dwFileSize,
   					block[i].fileName);
		}
	}
}

void PrintFileInfo(LPTSTR currPath, int files) {
	int i;
	strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    if (currPath) {
    	for (i = 0; i < files; i++) {
    		if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
    			if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
    				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<SYMLINKD>\t%s\n"), 
    					dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
    					dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
    			}

    			else {
    				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%s\n"), 
    					dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
    					dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
    			}
    		}

    		if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
    			continue;

    		if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
   				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%d\t%s\n"), 
   					block[i].wMonth, block[i].wDay, block[i].wYear, 
   					block[i].wHour, block[i].wMinute, block[i].dwFileSize,
   					block[i].fileName);
    		}
    	}
    }
}

void PrintThousandFormat(LPTSTR currPath, int files, LPTSTR option) {
    int i;
    strtok(currPath, "*");
    printf("\n%s\n\n", currPath);

    if (lstrcmpA(option, "/C") == 0) {
        for (i = 0; i < files; i++) {
            if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
                if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
    				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<SYMLINKD>\t%s\n"), 
    					dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
    					dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
                }

                else {
    				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%s\n"), 
    					dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
    					dirBlock[i].wHour, dirBlock[i].wMinute, dirBlock[i].dirName);
                }
            }

            RemoveDot(block[i].fileThousandSpr);

            if (block[i].dwFileAttribute & FILE_ATTRIBUTE_HIDDEN)
                continue;

            if (block[i].dwFileAttribute & FILE_ATTRIBUTE_ARCHIVE) {
   				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t%s\t%s\n"), 
   					block[i].wMonth, block[i].wDay, block[i].wYear, 
   					block[i].wHour, block[i].wMinute, block[i].fileThousandSpr,
   					block[i].fileName);
            }
        }
    }
}

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

void PrintSortedFile(LPTSTR currPath, int files) {
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
				block[i].wMonth, block[i].wDay, block[i].wYear, 
				block[i].wHour, block[i].wMinute, block[i].fileThousandSpr,
				block[i].fileName);
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

void PrintShortName(LPTSTR currPath, int files) {
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
    					dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
    					dirBlock[i].wHour, dirBlock[i].wMinute, 15,
    					dirBlock[i].alternateName, dirBlock[i].dirName);
            	}
            }

            else {
            	if (dirBlock[i].dirName != NULL) {
   					_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%*s\t%s\n"), 
   						dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
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
   					block[i].wMonth, block[i].wDay, block[i].wYear, 
   					block[i].wHour, block[i].wMinute, block[i].dwFileSize,
   					15, block[i].alternateName, block[i].fileName);
            }
        }
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

void PrintFileOwner(LPTSTR currPath, int files) {
	int i;
	strtok(currPath, "*");
	printf("\n%s\n\n", currPath);

	for (i = 0; i < files; i++) {
		if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_DIRECTORY) {
			if (dirBlock[i].dwDirAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
   				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<SYMLINKD>\t%s/%s   %s\n"), 
   					dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
   					dirBlock[i].wHour, dirBlock[i].wMinute, 
   					dirBlock[i].accountOwner, dirBlock[i].ownerMachine,
   					dirBlock[i].dirName);

			}
			else {
   				_tprintf(TEXT("%02d/%02d/%02d\t%02d:%02d\t<DIR>\t%s/%s   %s\n"), 
   					dirBlock[i].wMonth, dirBlock[i].wDay, dirBlock[i].wYear,
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
				block[i].wMonth, block[i].wDay, block[i].wYear, 
				block[i].wHour, block[i].wMinute, block[i].dwFileSize,
				block[i].accountOwner, block[i].ownerMachine,
				block[i].fileName);
		}
	}
}

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

void FindDirectories(LPTSTR lpFolder) {
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

void PauseFileOutput(LPTSTR currPath, int files) {
	int c = 0;
	int i = 0;
	
	if (files > 20) {
        for (i = 0; i < files; i++) {
            int result;
            PrintFileInfo(currPath, i);
            if(getchar() != 27) {
                printf("Press enter to continue...\n");
            }
        }
	}

	PrintFileInfo(currPath, files);
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
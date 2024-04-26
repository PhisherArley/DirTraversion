#include "fileinformation.h"

void DirAttributes(LPTSTR currentDir, LPTSTR option) {
	WIN32_FIND_DATA FileData;
	TCHAR szDir[MAX_PATH];
	HANDLE hSearch;
	StringCchCopy(szDir, MAX_PATH, currentDir);
	strtok(currentDir, "*");
	printf("\n%s\n\n", currentDir);
	hSearch = FindFirstFile(szDir, &FileData);

	if (hSearch == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("No files found\n"));
	}

	while (FindNextFile(hSearch, &FileData) != 0) {
		if ((lstrcmpA(option, "/A:D") == 0) && 
			(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			_tprintf(TEXT("%s\n"), FileData.cFileName);
		}

		if ((lstrcmpA(option, "/A:H") == 0) && 
			(FileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
			_tprintf(TEXT("%s\n"), FileData.cFileName);
		}

		if ((lstrcmpA(option, "/A:S") == 0) && 
			(FileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) {
			_tprintf(TEXT("%s\n"), FileData.cFileName);
		}

		if ((lstrcmpA(option, "/A:L") == 0) && 
			(FileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
			_tprintf(TEXT("%s\n"), FileData.cFileName);
		}

		if ((lstrcmpA(option, "/A:R") == 0) && 
			(FileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
			_tprintf(TEXT("%s\n"), FileData.cFileName);
		}

		if ((lstrcmpA(option, "/A:A") == 0) && 
			(FileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)) {
			_tprintf(TEXT("%s\n"), FileData.cFileName);
		}

		if ((lstrcmpA(option, "/A:I") == 0) && 
			(FileData.dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)) {
			_tprintf(TEXT("%s\n"), FileData.cFileName);
		}
	}

	FindClose(hSearch);
}

int DirSortOrder(LPTSTR currentDir, LPTSTR option, int files) {
	if ((lstrcmpA(option, "/O:D")) == 0) {
		SortDirAndFile(files);
		PrintDirFirst(currentDir, files);
		return 0;
	}

	if ((lstrcmpA(option, "/O:S")) == 0) {
		DescendingOrderSort(files);
		PrintDirFirst(currentDir, files);
		return 0;
	}

	if ((lstrcmpA(option, "/O:G")) == 0) {
		PrintDirFirst(currentDir, files);
		return 0;
	}

	if ((lstrcmpA(option, "/O:E")) == 0) {
		PrintFileExt(currentDir, files);
		return 0;
	}

	if ((lstrcmpA(option, "/O:N")) == 0) {
		PrintSortedFile(currentDir, files);
		return 0;
	}

	return 1;
}

int DirTimeField(LPTSTR currentDir, LPTSTR option, int files) {
	int i;
	if (lstrcmpA(option, "/T:C") == 0) {
		if(GetTimeField(currentDir, option) < 1) {
			_tprintf(TEXT("Error in DirTimeField: "
				"Could not execute GetTimeField properly\n"));
			return 1;
		}

		PrintFileInfo(currentDir, files);
		return 0;
	}

	if (lstrcmpA(option, "/T:A") == 0) {
		if(GetTimeField(currentDir, option) < 1) {
			_tprintf(TEXT("Error in DirTimeField: "
				"Could not execute GetTimeField properly\n"));
			return 1;
		}

		PrintFileInfo(currentDir, files);
		return 0;
	}

	if (lstrcmpA(option, "/T:W") == 0) {
		if(GetTimeField(currentDir, option) < 1) {
			_tprintf(TEXT("Error in DirTimeField: "
				"Could not execute GetTimeField properly\n"));
			return 1;
		}

		PrintFileInfo(currentDir, files);
		return 0;
	}

	return 1;
}

void DirNormalOptions(LPTSTR currentDir, LPTSTR option, int files) {
	if (lstrcmpA(option, "/B") == 0) {
		PrintBareFile(currentDir, files);
	}

    if (lstrcmpA(option, "/C") == 0) {
        int thousands = 0;
        int i = 0;
        thousands = SetThousandSeparator(files);
        if (!thousands) {
            _tprintf(TEXT("Could not execute SetThousandSeparator (%d)\n"),
                GetLastError());
        }

        PrintThousandFormat(currentDir, files, option);
    }

	if (lstrcmpA(option, "/D") == 0) {
		PrintFileInColumn(currentDir, files);
	}

	if (lstrcmpA(option, "/P") == 0) {
		PauseFileOutput(currentDir, files);
	}

	if (lstrcmpA(option, "/Q") == 0) {
		GetFileOwner(currentDir);
		PrintFileOwner(currentDir, files);
	}

    if (lstrcmpA(option, "/S") == 0) {
        FindDirectories(currentDir);
    }

	if (lstrcmpA(option, "/W") == 0) {
		PrintFileInWideFormat(currentDir, files);
	}

    if (lstrcmpA(option, "/X") == 0) {
        PrintShortName(currentDir, files);
    }

}

int ProcessOptions(LPTSTR targetDir, LPTSTR option, int files) {
	int processed;
	int i;
	LPTSTR attributeOptions[7] = {"/A:D", "/A:H", "/A:S", "/A:L", "/A:R", "/A:A", "/A:I"};
	LPTSTR sortOrder[5] = {"/O:N", "/O:E", "/O:G", "/O:S", "/O:D"};
	LPTSTR timeField[3] = {"/T:C", "/T:A", "/T:W"};    // Fix hours and minutes

	// Incomplete output in: /Q
	LPTSTR normalOptions[9] = {"/B", "/C", "/D", "/P", "/Q", "/S", "/W", "/X"};
	
	for (i = 0; i <= 6; i++) {
		if (lstrcmpA(option, attributeOptions[i]) == 0) {
			StringCchCat(targetDir, MAX_PATH, "\\*");
			DirAttributes(targetDir, option);
		}
	}
	
	for (i = 0; i <= 4; i++) {
		if (lstrcmpA(option, sortOrder[i]) == 0) {
			if (DirSortOrder(targetDir, option, files))
				printf("Error in DirSortOrder\n");
		}
	}
	
	for (i = 0; i <= 2; i++) {
		if (lstrcmpA(option, timeField[i]) == 0) {
			if (DirTimeField(targetDir, option, files))
				printf("Error in DirTimeField\n");
		}
	}
	
	for (i = 0; i <= 8; i++) {
		if (lstrcmpA(option, normalOptions[i]) == 0) {
			DirNormalOptions(targetDir, option, files);
		}
	}
	
	return 0;
}

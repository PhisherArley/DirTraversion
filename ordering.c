#include "fileinformation.h"

int CompareSmaller(const void *a, const void *b) {
    const FileInformation *arg1 = a;
    const FileInformation *arg2 = b;

    return arg1->dwFileSize < arg2->dwFileSize ? -1 : 1;
}

int CompareGreater(const void *a, const void *b) {
    const FileInformation *arg1 = a;
    const FileInformation *arg2 = b;

    return arg1->dwFileSize > arg2->dwFileSize ? -1 : 1;
}

int CompareFileDate(const void *a, const void *b) {
    const FileInformation *arg1 = a;
    const FileInformation *arg2 = b;

    if (arg1->wYear < arg2->wYear)
        return -1;

    if (arg1->wYear == arg2->wYear && arg1->wMonth < arg2->wMonth)
        return -1;

    if (arg1->wYear == arg2->wYear && arg1->wMonth == arg2->wMonth && arg1->wDay < arg2->wDay)
        return -1;

    return 1;
}

int CompareDirDate(const void *a, const void *b) {
    const DirInformation *arg1 = a;
    const DirInformation *arg2 = b;

    if (arg1->wYear < arg2->wYear)
        return -1;

    if (arg1->wYear == arg2->wYear && arg1->wMonth < arg2->wMonth)
        return -1;

    if (arg1->wYear == arg2->wYear && arg1->wMonth == arg2->wMonth && arg1->wDay < arg2->wDay)
        return -1;

    return 1;
}

void RemoveDot(TCHAR *thousandsArr) {
    TCHAR *token;

    if (thousandsArr == NULL)
        return;

    token = strtok(thousandsArr, ".");
    while (token != NULL)
        token = strtok(NULL, ".");
}

void DescendingOrderSort(int files) {
    qsort(block, files, sizeof(FileInformation), CompareSmaller);
}

void AscendingOrderSort(int files) {
    qsort(block, files, sizeof(FileInformation), CompareGreater);
}

void SortFileDate(int files) {
    qsort(block, files, sizeof(FileInformation), CompareFileDate);
}

void SortDirDate(int files) {
    qsort(dirBlock, files, sizeof(DirInformation), CompareDirDate);
}

void SortDirAndFile(int files) {
    SortDirDate(files);
    SortFileDate(files);
}

#include <windows.h>

#include "logs.h"

int MdbgCreateLogFile(
    _In_  char*   fileName,
    _Out_ HANDLE* hFile)
{
    *hFile = CreateFileA(fileName, GENERIC_WRITE, 0, 
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (*hFile == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }

    return 0;
}

void MdbgCloseLogFile(
    _In_ HANDLE hFile)
{
    if (hFile) {
        CloseHandle(hFile);
    }

    return;
}

static
int MdbgWriteLogToFile(
    _In_ HANDLE hFile,
    _In_ void*  buf,
    _In_ DWORD  size)
{
    DWORD wsize;

    if (!WriteFile(hFile, buf, size, &wsize, NULL)) {
        return GetLastError();
    }

    return 0;
}

static
char* AddString(
    _In_ char*   dststr,
    _In_ size_t* dststrSize,
    _In_ char*   srcstr)
{
    size_t freeSize = *dststrSize - strlen(dststr);
    size_t srcstrLen = strlen(srcstr);

    if (freeSize <= srcstrLen) {
        ++srcstrLen;
        *dststrSize += srcstrLen;
        dststr = (char*)realloc(dststr, *dststrSize);
    }

    strcat_s(dststr, *dststrSize, srcstr);

    return dststr;
}


static
char* BuildLogMsg(
    _In_     char*  msg,
    _In_opt_ char*  fileAllocatedMem,
    _In_opt_ size_t lineAllocatedMem,
    _In_opt_ char*  fileFailedMem,
    _In_opt_ size_t lineFailedMem)
{
    char*  msgBuf;
    size_t bufSize;
    char   line[16];

    if (!msg) {
        return NULL;
    }

    bufSize = strlen(msg);
    if (fileAllocatedMem) {
        bufSize += strlen(fileAllocatedMem);
    }
    if (fileFailedMem) {
        bufSize += strlen(fileFailedMem);
    }
    ++bufSize;

    msgBuf = (char*)malloc(bufSize);
    if (!msgBuf) {
        return NULL;
    }

    strcpy_s(msgBuf, bufSize, msg);
    if (fileFailedMem) {
        msgBuf = AddString(msgBuf, &bufSize, fileFailedMem);
        msgBuf = AddString(msgBuf, &bufSize, delimiter);
        if (_itoa_s((int)lineFailedMem, line, 0x10, 0xA)) {
            free(msgBuf);
            return NULL;
        }
        msgBuf = AddString(msgBuf, &bufSize, line);
    }
    msgBuf = AddString(msgBuf, &bufSize, car_ret_1);

    if (fileAllocatedMem) {
        msgBuf = AddString(msgBuf, &bufSize, memory_alloc);
        msgBuf = AddString(msgBuf, &bufSize, fileAllocatedMem);
        msgBuf = AddString(msgBuf, &bufSize, delimiter);
        if (_itoa_s((int)lineAllocatedMem, line, 0x10, 0xA)) {
            free(msgBuf);
            return NULL;
        }
        msgBuf = AddString(msgBuf, &bufSize, line);
        msgBuf = AddString(msgBuf, &bufSize, car_ret_1);
    }

    msgBuf = AddString(msgBuf, &bufSize, car_ret_2);

    return msgBuf;
}

int MdbgLoggingBug(
    _Inout_  HANDLE* hFile,
    _In_     char*   fileAllocatedMem,
    _In_     size_t  lineAllocatedMem,
    _In_opt_ char*   fileFailedMem,
    _In_opt_ size_t  lineFailedMem,
    _In_     size_t  bugFlag)
{
    char* logBuf;
    int   err;

    if (bugFlag != UNALLOC_MEMORY && !fileAllocatedMem) {
        return STATUS_INVALID_PARAMETER;
    }

    switch (bugFlag) {
    case MEMORY_LEAK:
        logBuf = BuildLogMsg(mem_leaks_bug, fileAllocatedMem,
            lineAllocatedMem, NULL, 0);
        break;
    case OVERFLOW:
        if (!fileFailedMem) {
            logBuf = BuildLogMsg(buf_overflow_bug, fileAllocatedMem,
                lineAllocatedMem, unknown_file, 0);
        } else {
            logBuf = BuildLogMsg(buf_overflow_bug, fileAllocatedMem,
                lineAllocatedMem, fileFailedMem, lineFailedMem);
        }
        break;
    case UNALLOC_MEMORY:
        if (!fileFailedMem) {
            return STATUS_INVALID_PARAMETER;
        }
        logBuf = BuildLogMsg(access_mem_bug, NULL, 0, fileFailedMem, 
            lineFailedMem);
        break;
    default:
        logBuf = NULL;
        break;
    }

    if (logBuf == NULL) {
        return STATUS_NO_MEMORY;
    }

    if (!*hFile) {
        err = MdbgCreateLogFile(LOG_FILE, hFile);
        if (err) {
            goto _freeBuf;
        }
    }

    err = MdbgWriteLogToFile(*hFile, logBuf, (DWORD)strlen(logBuf));
_freeBuf:
    free(logBuf);
    return err;
}

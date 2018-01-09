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
    _In_ DWORD size)
{
    DWORD wsize;

    if (!WriteFile(hFile, buf, size, &wsize, NULL)) {
        return GetLastError();
    }

    return 0;
}

static
char* BuildLogMsg(
    _In_     char*        msg,
    _In_opt_ char*        fileAllocatedMem,
    _In_opt_ unsigned int lineAllocatedMem,
    _In_opt_ char*        fileFailedMem,
    _In_opt_ unsigned int lineFailedMem,
    _In_     unsigned int bufSize)
{
    char* msgBuf;
    char  line[16];

    msgBuf = (char*)malloc(bufSize += strlen(car_ret_2));
    if (!msgBuf) {
        return NULL;
    }
    memset(msgBuf, 0, bufSize);

    strcpy_s(msgBuf, bufSize, msg);
    if (fileFailedMem) {
        strcat_s(msgBuf, bufSize, fileFailedMem);
        strcat_s(msgBuf, bufSize, delimiter);
        if (_itoa_s(lineFailedMem, line, 0x10, 0xA)) {
            free(msgBuf);
            return NULL;
        }
        strcat_s(msgBuf, bufSize, line);
        strcat_s(msgBuf, bufSize, car_ret_1);
    }

    if (fileAllocatedMem) {
        strcat_s(msgBuf, bufSize, memory_alloc);
        strcat_s(msgBuf, bufSize, fileAllocatedMem);
        strcat_s(msgBuf, bufSize, delimiter);
        if (_itoa_s(lineAllocatedMem, line, 0x10, 0xA)) {
            free(msgBuf);
            return NULL;
        }
        strcat_s(msgBuf, bufSize, line);
        strcat_s(msgBuf, bufSize, car_ret_1);
    }

    strcat_s(msgBuf, bufSize, car_ret_2);

    return msgBuf;
}

int MdbgLoggingBug(
    _Inout_  HANDLE*      hFile,
    _In_     char*        fileAllocatedMem,
    _In_     unsigned int lineAllocatedMem,
    _In_opt_ char*        fileFailedMem,
    _In_opt_ unsigned int lineFailedMem,
    _In_     unsigned int bugFlag)
{
    char*        logBuf;
    unsigned int bufSize = MIN_MSG_SIZE;
    int          err;

    if (!fileAllocatedMem) {
        return STATUS_INVALID_PARAMETER;
    }

    bufSize += strlen(fileAllocatedMem);

    switch (bugFlag) {
    case MEMORY_LEAK:
        logBuf = BuildLogMsg(mem_leaks_bug, NULL, 0, fileAllocatedMem,
            lineAllocatedMem, bufSize);
        break;
    case OVERFLOW:
        if (!fileFailedMem) {
            bufSize += strlen(unknown_file);
            logBuf = BuildLogMsg(buf_overflow_bug, fileAllocatedMem,
                lineAllocatedMem, unknown_file, 0, bufSize);
        } else {
            bufSize += strlen(fileFailedMem);
            logBuf = BuildLogMsg(buf_overflow_bug, fileAllocatedMem,
                lineAllocatedMem, fileFailedMem, lineFailedMem, bufSize);
        }
        break;
    case UNALLOC_MEMORY:
        if (fileFailedMem) {
            return STATUS_INVALID_PARAMETER;
        }
        bufSize += strlen(fileFailedMem);
        logBuf = BuildLogMsg(unalloc_mem_bug, NULL, 0, fileFailedMem, 
            lineFailedMem, bufSize);
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

    err = MdbgWriteLogToFile(*hFile, logBuf, strlen(logBuf));
_freeBuf:
    free(logBuf);
    return err;
}

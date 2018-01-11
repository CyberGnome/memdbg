#include <windows.h>
#include <stdio.h>

#include "debug.h"

/*****************************************************************************/
/*                               DESCRIPTION                                 */
/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* THIS APPLICATION HAS MANY BUGS WITH MEMORY.                               */
/* THIS IS DONE TO DEMONSTRATE THE CAPABILITIES OF THE LIBRARY "MEMDBG.LIB". */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/



void CalcFactorial(
    _In_  size_t  num,
    _Out_ size_t* resInt,
    _Out_ char**  resStr)
{
    size_t i;
    size_t res = 1;

    if (num > 1) {
        for (i = 2; i <= num; ++i) {
            res *= i;
        }
    }

    *resInt = res;
    *resStr = (char*)MALLOC(sizeof(size_t));
    if (!*resStr) {
        return;
    }

    itoa((int)res, *resStr, 0xA);
    return;
}

void BuildResultMsg(
    _In_ char* msg,
    _In_ int   msgSize,
    _In_ char* num,
    _In_ char* fact_str)
{
    char* str = " is factorial of number ";

    MEMCPY(msg, fact_str, strlen(fact_str));
    MEMCPY(&msg[strlen(msg)], str, strlen(str));
    MEMCPY_S(&msg[strlen(msg)], msgSize - strlen(msg), 
        num, strlen(num));

    return;
}

void PrintFactorial(
    _In_ size_t num)
{
    size_t res;
    char*  resStr;
    char*  msg;
    char   numStr[10];

    itoa((int)num, numStr, 0xA);

    msg = (char*)CALLOC(32, 1);
    if (!msg) {
        return;
    }

    CalcFactorial(num, &res, &resStr);
    if (resStr) {
        BuildResultMsg(msg, 32, numStr, resStr);

        msg = (char*)REALLOC(msg, 48);
        if (msg) {
            printf("%s\n", msg);
        }
    }

    FREE(msg);
    return;
}


BOOLEAN FillFive(
    _In_ void*  buf,
    _In_ size_t cols)
{
    void* tmp = MALLOC(cols);

    if (!tmp) {
        return FALSE;
    }

    MEMSET(tmp, 5, cols);

    MEMMOVE(buf, tmp, cols);

    FREE(tmp);

    return TRUE;
}

int main(void)
{
    void* buf1;
    void* buf2;

    MEMDBG_INIT();

    PrintFactorial(5);
    PrintFactorial(10);
    PrintFactorial(25);

    buf1 = MALLOC(50);
    if (!buf1) {
        return 0;
    }
    buf2 = MALLOC(100);
    if (!buf2) {
        return 0;
    }

    FillFive(buf1, 50);
    FillFive(buf2, 200);

    FREE(buf1);
    FREE(buf2);

    MEMDBG_DEINIT();

    return 0;
}

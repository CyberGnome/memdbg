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
    _In_ char* num,
    _In_ char* fact_str)
{
    char* str = " is factorial of number ";

    MEMCPY(msg, fact_str, strlen(fact_str));
    MEMCPY(&msg[strlen(msg)], str, strlen(str));
    MEMCPY(&msg[strlen(msg)], num, strlen(num));

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
        BuildResultMsg(msg, numStr, resStr);
        printf("%s\n", msg);
    }

    return;
}

int main(void)
{

    MEMDBG_INIT();

    PrintFactorial(5);
    PrintFactorial(10);
    PrintFactorial(25);

    MEMDBG_DEINIT();

    return 0;
}


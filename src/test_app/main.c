#include <windows.h>

#include "memdbg.h"

int main(void)
{
    void* buf;
    char* test_str = "Hello World!";

    memdbg_init();

    buf = dbg_calloc(4, 2);

    dbg_memcpy((void*)((size_t)buf + 4), test_str, 5);

    dbg_free(buf);
    memdbg_deinit();
    return 0;
}

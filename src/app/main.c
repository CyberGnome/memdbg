#include <windows.h>

#include "memdbg.h"

int main(void)
{
    void* buf;

    memdbg_init();

    buf = dbg_malloc(9);
    buf = dbg_malloc(9);
    memset(buf, 0, 10);
    dbg_free(buf);

    memdbg_deinit();

    return 0;
}

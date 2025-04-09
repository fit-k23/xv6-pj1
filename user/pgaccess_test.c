#include "kernel/types.h"
#include "kernel/riscv.h"
#include "user.h"

void 
pgaccess_test()
{
    char *buf;
    uint64 bitmask;

    printf("Detecting page access!\n");

    // allocate virtual addresses for user program
    buf = malloc(32 * PGSIZE);
    memset(buf, 0, sizeof(buf));

    // Set valid bit for some pages (allow to access)
    int allow[] = {2, 5, 6, 10};
    for (int i = 0; i < sizeof(allow)/sizeof(int); ++i)
        buf[allow[i] * PGSIZE] += 1;

    if (pageaccess((uint64)buf, 32, (uint64)&bitmask) < 0) {
        printf("Fail in page access test!\n");
        free(buf);
        return;
    }

    printf("Bitmask of result: ");
    for (int i = 32; i >= 0; --i) {
        if (bitmask & (1 << i))
            printf("%d", 1);
        else printf("%d", 0);
    }
    printf("\n");

    // Check invalid page access
    int invalid = 0;
    for (int i = 0; i < sizeof(allow)/sizeof(int); ++i)
        if (!(bitmask & (1 << allow[i])))
            invalid = 1;

    if (invalid) {
        printf("Inaccurate accessed page!\n");
        free(buf);
        return;
    }

    free(buf);
    printf("Successful accessed page test!\n");
}

int 
main(int argc, char** argv) 
{
    pgaccess_test();
    return 0;
}
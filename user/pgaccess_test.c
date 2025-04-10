#include "kernel/types.h"
#include "kernel/riscv.h"
#include "user.h"

void 
pgaccess_test()
{
    char *buf;
    uint64 bitmask;

    printf("Detecting accessed page...\n");

    // allocate 32 pages for user program
    // use the malloc function in umalloc.c/line 64
    buf = malloc(32 * PGSIZE);

    // Access to some pages to trigger their access bit 
    // The 2-th and 3-th value tries accessing within the page for more ensured check
    // The first page of the virtual memory (page 0) in this program will always be accessed, because we pass the pointer buf
    // to system call sys_pgaccess(), which reads the address of buf[0].
    int valid[] = {2 * PGSIZE, 5 * PGSIZE, 6 * PGSIZE + 25, 10 * PGSIZE + 5};
    int want[] = {0, 2, 5, 6, 10};
    for (int i = 0; i < sizeof(valid)/sizeof(int); ++i)
        buf[valid[i]] = '2';

    if (pageaccess((uint64)buf, 32, (uint64)&bitmask) < 0) {
        printf("Fail in page access system call!\n");
        free(buf);
        return;
    }

    printf("Result bitmask: ");
    for (int i = 31; i >= 0; --i) {
        if (bitmask & (1 << i)) 
            printf("%d", 1);
        else printf("%d", 0);
    }
    printf("\n");

    for (int i = 31; i >= 0; --i) {
        if (bitmask & (1 << i)) 
            printf("page %d is accessed\n", i);
    }

    // Check invalid page access
    int invalid = 0;
    for (int i = 0; i < sizeof(want)/sizeof(int); ++i)
        if (!(bitmask & (1 << want[i])))
            invalid = 1;
    if (invalid) {
        printf("Inaccurate result!\n");
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
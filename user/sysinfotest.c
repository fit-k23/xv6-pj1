#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user.h"

int main(int argc, char** argv) {
    if (argc > 1) {
        printf("Sysinfotest usage: sysinfotest\n");
        exit(1);
    }
    struct sysinfo sysinf;
    info(&sysinf);

    printf("Free memory: %lu bytes\nUnused processes: %d\nOpen files: %d\n", sysinf.freemem, sysinf.nproc, sysinf.nopenfiles);
    exit(1);
}
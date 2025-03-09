#include "kernel/types.h"
struct sysinfo {
    uint64 freemem;
    uint nproc;
    uint nopenfiles;
};
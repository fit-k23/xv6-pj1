#include "kernel/types.h"
#include "user.h"

int main(int argc, char** argv) {
    int mask = atoi(argv[1]);
    if (argc < 3) {
        printf("Trace usage: trace <call number> <command>\n");
        exit(1);
    }
    trace(mask);
    exec(argv[2], &argv[2]);
    exit(1);
}
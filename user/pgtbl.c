#include "kernel/types.h"
#include "user.h"

int
main(int argc, char** argv)
{
  if (argc < 2) {
    printf("Usage: pgtbl <command> [args...]\n");
    exit(1);
  }
  flagpgtbl();
  if (exec(argv[1], &argv[1]) < 0) {
    printf("pgtbl: exec %s failed\n", argv[1]);
  }
  exit(0);
}

#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int prio = getpriority();
  printf("initial priority: %d\n", prio);

  if (setpriority(15) < 0) {
    printf("setpriority(15) failed\n");
    exit(1);
  }
  printf("priority after setpriority(15): %d\n", getpriority());

  if (setpriority(50) == 0) {
    printf("setpriority(50) should fail (max is 49)!\n");
  } else {
    printf("setpriority(50) properly rejected (valid range 0-49)\n");
  }

  exit(0);
}

#include "kernel/types.h"
#include "user/user.h"

void
do_work(int id, int prio)
{
  setpriority(prio);
  printf("Process %d started (priority %d)\n", id, prio);

  volatile int x = 0;
  for (int i = 0; i < 50000000; i++) {
    x += 1;
  }

  printf("Process %d FINISHED (priority %d)\n", id, prio);
  exit(0);
}

int
main(int argc, char *argv[])
{
  printf("--- Starting Priority Scheduler Test ---\n");

  int pid1 = fork();
  if (pid1 == 0) {
    do_work(1, 5); // Low priority
  }

  int pid2 = fork();
  if (pid2 == 0) {
    do_work(2, 20); // High priority
  }

  wait(0);
  wait(0);

  printf("--- Priority Scheduler Test Completed ---\n");
  exit(0);
}

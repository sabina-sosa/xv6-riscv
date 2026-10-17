#include "kernel/types.h"
#include "user/user.h"

int pfd[2];

void
do_work(int id, int prio)
{
  setpriority(prio);

  // Wait until parent signals so both children start at the exact same time
  char c;
  read(pfd[0], &c, 1);
  close(pfd[0]);

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

  if (pipe(pfd) < 0) {
    printf("pipe failed\n");
    exit(1);
  }

  int pid1 = fork();
  if (pid1 == 0) {
    close(pfd[1]);
    do_work(1, 5); // Low priority
  }

  int pid2 = fork();
  if (pid2 == 0) {
    close(pfd[1]);
    do_work(2, 20); // High priority
  }

  // Brief pause to make sure both children are waiting
  pause(2);

  // Release both children simultaneously
  write(pfd[1], "xx", 2);
  close(pfd[1]);
  close(pfd[0]);

  wait(0);
  wait(0);

  printf("--- Priority Scheduler Test Completed ---\n");
  exit(0);
}

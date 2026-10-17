#include "kernel/types.h"
#include "user/user.h"

int pfd[2];

void
work(int id, int prio, int loop_count)
{
  setpriority(prio);

  // Synchronize start
  char c;
  read(pfd[0], &c, 1);
  close(pfd[0]);

  int start = uptime();
  printf("Process %d (base priority %d) STARTED at tick %d\n", id, prio, start);

  volatile int x = 0;
  for (int i = 0; i < loop_count; i++) {
    x += 1;
    if (i % (loop_count / 2) == 0 && i > 0) {
      printf("Process %d (base priority %d) PROGRESS at tick %d\n", id, prio, uptime());
    }
  }

  int end = uptime();
  printf("Process %d (base priority %d) FINISHED at tick %d (turnaround: %d ticks)\n",
         id, prio, end, end - start);
  exit(0);
}

int
main(int argc, char *argv[])
{
  printf("--- Starting Aging Scheduler Test ---\n");

  if (pipe(pfd) < 0) {
    printf("pipe failed\n");
    exit(1);
  }

  // Process 1: Low priority (base 5), medium work
  int pid1 = fork();
  if (pid1 == 0) {
    close(pfd[1]);
    work(1, 5, 40000000);
  }

  // Process 2: High priority (base 30), long work
  int pid2 = fork();
  if (pid2 == 0) {
    close(pfd[1]);
    work(2, 30, 80000000);
  }

  // Let both children get created and wait on pipe
  pause(2);

  // Release both children simultaneously
  write(pfd[1], "xx", 2);
  close(pfd[1]);
  close(pfd[0]);

  wait(0);
  wait(0);

  printf("--- Aging Scheduler Test Completed ---\n");
  exit(0);
}

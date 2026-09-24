#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: time command [args...]\n");
    exit(1);
  }

  int start = uptime();

  int pid = fork();
  if(pid < 0){
    fprintf(2, "time: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    // child: run the given command
    exec(argv[1], argv + 1);
    fprintf(2, "time: exec %s failed\n", argv[1]);
    exit(1);
  }

  // parent: wait for child and collect cputime
  struct rusage ru;
  wait2(0, &ru);
  int elapsed = uptime() - start;
  int pct = elapsed > 0 ? ru.cputime * 100 / elapsed : 0;
  printf("elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n",
         elapsed, ru.cputime, pct);
  exit(0);
}

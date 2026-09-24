#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: time1 command [args...]\n");
    exit(1);
  }

  int start = uptime();

  int pid = fork();
  if(pid < 0){
    fprintf(2, "time1: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    // child: run the given command
    exec(argv[1], argv + 1);
    fprintf(2, "time1: exec %s failed\n", argv[1]);
    exit(1);
  }

  // parent: wait for child then print elapsed time
  wait(0);
  printf("elapsed time: %d ticks\n", uptime() - start);
  exit(0);
}

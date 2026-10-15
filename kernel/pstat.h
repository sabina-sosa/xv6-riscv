#ifndef _PSTAT_H_
#define _PSTAT_H_

#ifndef _PROCSTATE_
#define _PROCSTATE_
enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
#endif

struct rusage {
  uint cputime;
};

struct pstat {
  int pid;              // Process ID
  enum procstate state; // Process state
  uint64 size;          // Size of process memory (bytes)
  int ppid;             // Parent process ID
  char name[16];        // Process command name
};

#endif

#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  struct pstat uproc[NPROC];
  int nprocs;
  int i;
  char *state;
  static char *states[] = {
    [SLEEPING]  "sleeping",
    [RUNNABLE]  "runnable",
    [RUNNING]   "running ",
    [ZOMBIE]    "zombie  "
  };

  nprocs = getprocs(uproc);
  if (nprocs < 0)
    exit(-1);

  int now = uptime();

  printf("pid\tstate\t\tsize\tage\tpriority\tppid\tname\n");
  for (i=0; i<nprocs; i++) {
    state = states[uproc[i].state];
    int age = 0;
    if (uproc[i].state == RUNNABLE)
      age = now - uproc[i].readytime;
    printf("%d\t%s\t%lu\t%d\t%d\t\t%d\t%s\n", uproc[i].pid, state,
                   uproc[i].size, age, uproc[i].priority, uproc[i].ppid, uproc[i].name);
  }

  exit(0);
}


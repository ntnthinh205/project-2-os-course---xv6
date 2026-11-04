#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

void
sbrk_test()
{
  struct sysinfo info;

  printf("sysinfotest: start sbrk_test\n");

  if (sysinfo(&info) < 0) {
    printf("sysinfotest: sysinfo failed\n");
    exit(1);
  }

  uint64 freemem_before = info.freemem;

  // Allocate a page
  char *p = sbrk(PGSIZE);
  if (p == (char *)-1) {
    printf("sbrk failed\n");
    exit(1);
  }

  if (sysinfo(&info) < 0) {
    printf("sysinfotest: sysinfo failed\n");
    exit(1);
  }

  uint64 freemem_after = info.freemem;

  if (freemem_before - freemem_after != PGSIZE) {
    printf("sbrk_test failed: freemem_before=%d freemem_after=%d, expected diff %d\n",
           (int)freemem_before, (int)freemem_after, PGSIZE);
    exit(1);
  }

  // Free the page
  sbrk(-PGSIZE);

  if (sysinfo(&info) < 0) {
    printf("sysinfotest: sysinfo failed\n");
    exit(1);
  }

  if (info.freemem != freemem_before) {
    printf("sbrk_test failed: freemem_after_dealloc=%d, expected %d\n",
           (int)info.freemem, (int)freemem_before);
    exit(1);
  }

  printf("sysinfotest: sbrk_test OK\n");
}

void
proc_test()
{
  struct sysinfo info;

  printf("sysinfotest: start proc_test\n");

  if (sysinfo(&info) < 0) {
    printf("sysinfotest: sysinfo failed\n");
    exit(1);
  }

  uint64 nproc_before = info.nproc;

  int pid = fork();
  if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    // child
    sleep(1);
    exit(0);
  } else {
    // parent
    if (sysinfo(&info) < 0) {
      printf("sysinfotest: sysinfo failed\n");
      exit(1);
    }

    uint64 nproc_after = info.nproc;

    if (nproc_after != nproc_before + 1) {
      printf("proc_test failed: nproc_before=%d nproc_after=%d, expected %d\n",
             (int)nproc_before, (int)nproc_after, (int)(nproc_before + 1));
      exit(1);
    }

    wait(0);

    if (sysinfo(&info) < 0) {
      printf("sysinfotest: sysinfo failed\n");
      exit(1);
    }

    if (info.nproc != nproc_before) {
      printf("proc_test failed: nproc_after_wait=%d, expected %d\n",
             (int)info.nproc, (int)nproc_before);
      exit(1);
    }
  }

  printf("sysinfotest: proc_test OK\n");
}

void
testcall()
{
  printf("sysinfotest: start testcall\n");

  if (sysinfo((struct sysinfo *)0xeaebc00000000000) != -1) {
    printf("sysinfotest: sysinfo succeeds with bad pointer\n");
    exit(1);
  }

  printf("sysinfotest: testcall OK\n");
}

int
main(int argc, char *argv[])
{
  printf("sysinfotest: start\n");
  testcall();
  sbrk_test();
  proc_test();
  printf("sysinfotest: OK\n");
  exit(0);
}

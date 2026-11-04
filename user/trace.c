#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  uint mask = 0;

  if(argc < 3 || argv[1][0] == 0){
    fprintf(2, "Usage: trace mask command [args ...]\n");
    exit(1);
  }

  // Accept decimal masks representable by a non-negative 32-bit int.
  for(char *p = argv[1]; *p; p++){
    if(*p < '0' || *p > '9' ||
       mask > (2147483647U - (*p - '0')) / 10){
      fprintf(2, "trace: invalid mask\n");
      exit(1);
    }
    mask = mask * 10 + (*p - '0');
  }

  if(trace((int)mask) < 0){
    fprintf(2, "trace: cannot enable tracing\n");
    exit(1);
  }
  exec(argv[2], &argv[2]);
  fprintf(2, "trace: exec %s failed\n", argv[2]);
  exit(1);
}

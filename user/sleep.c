#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int number = 0;
  if(argc < 2){
    fprintf(2, "Usage: sleep number...\n");
    exit(1);
  }

  number = atoi(argv[1]);
  sleep(number);

  exit(0);
}

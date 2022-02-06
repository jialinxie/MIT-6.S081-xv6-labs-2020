#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2], pid;
  char bufSend[1] = {0x55}, bufRecv[1];
  
  pipe(p);

  if(fork() == 0){
      read(p[0], bufRecv, 1);
      pid = getpid();
      fprintf(2, "%d: received ping\n", pid);
      write(p[1], bufSend, 1);
  }else{
      write(p[1], bufSend, 1);
      sleep(1);
      read(p[0], bufRecv, 1);
      pid = getpid();
      fprintf(2, "%d: received pong\n", pid);
  }

  exit(0);
}

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// #define debug

void pipe2right(char *data){
    int p[2];
    char *bufIn = 0;
    char bufOut[33] = {0};
    char basicPrime = 0;//first prime in pipe[0]
    int count = 0;
    bufIn = data;
    int len = strlen(bufIn);

#ifdef debug
    fprintf(2, "pipe2right pid = %d data len = %d\n", getpid(), len);
#endif
    pipe(p);
    basicPrime = bufIn[0];
    fprintf(2, "prime %d\n", basicPrime);
    for(int i = 0; i < len; i++){
        if(bufIn[i] <= basicPrime){
            continue;
        }
        if(bufIn[i] % basicPrime != 0){
            count ++;
            write(p[1], bufIn + i, 1);
#ifdef debug
            fprintf(2, "pid = %d count = %d write %d to right pipe\n", getpid(), count, bufIn[i]);
#endif
        }
    }

    if (fork() == 0)
    {
        read(p[0], bufOut, count);
        close(p[0]);
        close(p[1]);
#ifdef debug
        fprintf(2, "pid = %d, count = %d\n", getpid(), count);
        for (int i = 0; i < strlen(bufOut); i++)
        {
            fprintf(2, "child: %d recv: %d\n", getpid(), bufOut[i]);
        }
#endif
        pipe2right(bufOut);
    }else{
        sleep(1);
    }
};

int main(int argc, char *argv[])
{
    char buf[33] = {0};
    for(int i = 0; i < 33; i++){
        buf[i] = 2 + i;
    }

    pipe2right(buf);

    exit(0);
}

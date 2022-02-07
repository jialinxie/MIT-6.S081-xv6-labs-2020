// Lab Xv6 and Unix utilities
// xargs.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXN 1024

int
main(int argc, char *argv[])
{
    // 如果参数个数小于 2
    if (argc < 2) {
        // 打印参数错误提示
        fprintf(2, "usage: xargs command\n");
        // 异常退出
        exit(1);
    }

    char *argvs[MAXARG];
    char buf[32] = {'\n'};
    char temp[128] = {0};
    int index = 0, n = 0;

    //去掉xargs, 并拷贝所有参数到argvs
    for(int i = 1; i < argc; i++){
        argvs[index++] = argv[i];
    }

    argvs[index] = temp;
    //从标准输入读取参数列表
    while((n = read(0, buf, MAXN)) > 0){
        for(int i = 0; i < n; i++){
          if(buf[i] == '\n'){   // 读取单个输入行，当遇到换行符时，创建子线程
            if(fork() == 0){
              exec(argv[1], argvs);
            }
            wait(0);            // 等待子线程执行完毕
          }else{
            temp[i] = buf[i];
          }
        }
    }
    exit(0);
}
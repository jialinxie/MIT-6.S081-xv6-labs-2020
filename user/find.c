#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// #define DEBUG

/**
 * @brief 查找并返回path中最后一个'/'后的目录名字
 * 
 * @param path 
 * @return char* 
 */
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
  return buf;
}

void
find(char* path, char* name){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  char *fileName;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    close(fd);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    fileName = fmtname(path);
#ifdef DEBUG
    fprintf(2, "fileName = %s\n", fileName);
#endif
    if(strcmp(fileName, path) == 0){
        printf("%s %d %d %l\n", fileName, st.type, st.ino, st.size);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)  //空目录，跳过
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
#ifdef DEBUG
      fprintf(2, "de.name = %s, buf = %s\n", de.name, buf);
#endif
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }

      char *dirName = fmtname(buf);
      if(dirName[0] == '.'){
        continue;
      }

      if(st.type == T_DIR){
#ifdef DEBUG        
        fprintf(2, "catch T_DIR!\n");
#endif
        find(buf, name);
        continue;
      }
#ifdef DEBUG
      fprintf(2, "dirName = %s, len = %d findName = %s, len = %d\n", dirName, strlen(dirName), name, strlen(name));
#endif      
      if(strcmp(dirName, name) == 0){
#ifdef DEBUG
          printf("found -> %s %d %d %l\n", dirName, st.type, st.ino, st.size);
#endif
          fprintf(1, "%s\n", buf);
      }
    }
    break;
  }
  close(fd);
}


/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int
 *
 * 
echo > b
mkdir a
echo > a/b
find . b 
 */
int main(int argc, char* argv[]){
  if(argc < 3){
    fprintf(2, "parameter error! expect 3 parameters!\n");
    exit(0);
  }

  find(argv[1], argv[2]);
  exit(0);
}
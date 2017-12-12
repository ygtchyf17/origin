#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

char buf[512];

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
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}


void
cp1(int fd1, int fd2){

  int read_n;
  int write_n;

  /* fd1のファイルディスクリプタから文字を読み込む*/
  while((read_n = read(fd1, buf, sizeof(buf))) > 0){
    //buf[read_n] = '\0';

    /* fd2のファイルディスクリプタに読み込んだ文字を書き込む */
    write_n = write(fd2, buf, read_n);
    /* エラー処理 */
    if(write_n < 0){
      printf(1, "write: Error\n");
      exit();
    }

  }// whileの終端

  //printf(1, "\n");

  if(read_n < 0){
    printf(1, "cp1: cannot\n");
    exit();
  }


}

void strcat(char *to, char *from){
  while(*to)
    to++;

  while(*from)
    *to++ = *from++;

  *to = '\0';

}

int
main(int argc, char *argv[])
{
  int fd1, fd2;
  struct stat st;
  char str[80];
  //char *p;
  //struct dirent de;

  /* コマンドライン引数の確認 */
  if(argc <= 2){
    printf(1, "usage: cp1 filename1 filename2\n");
    exit();
  }

  /* コピー元ファイルをオープン */
  if((fd1 = open(argv[1], O_RDONLY)) < 0){
    printf(1, "cp1: cannot open %s\n", argv[1]);
    exit();
  }

  //unlink(argv[2]);
  /* コピー先ファイル/ディレクトリをオープン */
  fd2 = open(argv[2],  O_CREATE|O_RDWR);

  printf(1, "ここまでいったよ\nfd2 = %d\n", fd2);

  /* fstat --- ディレクトリの動作*/
  if(fstat(fd2, &st) < 0){

    //chdir(argv[2]);
    strcpy(str, argv[2]);
    //printf(1, "str:%s", str);
    strcat(str, "/");
    strcat(str, argv[1]);
    printf(1, "%s\n", str);
    fd2 = open(str, O_CREATE|O_RDWR);
    //chdir(argv[2]);
    cp1(fd1, fd2);

    printf(1, "できたよ\n");

    close(fd1);
    close(fd2);

    exit();
  }


  /* ファイルの動作 */
  cp1(fd1, fd2);


  close(fd1);
  close(fd2);


  exit();
}

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAXFILE 65535

int main()
{
    pid_t pc;
    int i,fd,len;
    char *buf="this is a dameon \n";
    len = strlen(buf);
    pc =fork();     //创建一个进程用来做守护进程
    if(pc<0)
    {
        printf("error fork \n");
        exit(1);
    }
    else if(pc>0) exit(0);    //结束父进程
    
    //使子进程独立1.摆脱原会话控制 2.摆脱原进程组的控制 3.摆脱控制中端的控制
    setsid();       
    //改变当前工作目录，这也是为了摆脱父进程的影响
    chdir("/");     
    //重设文件权限掩码
    umask(0);       
    //关闭文件描述符(常说的输入，输出，报错3个文件)，
    for(i=0;i<MAXFILE;i++)  
            close(i); //因为守护进程要失去了对所属的控制终端的联系，这三个文件要关闭

    while(1)
    {
      printf("printf\n");
      if((fd=open("/tmp/dameon.txt",O_CREAT|O_WRONLY|O_APPEND,0600))<0)
      {
        printf("open file err \n");
        exit(0);
      }
      write(fd,buf,len+1);
      close(fd);
      sleep(10);
    }
}

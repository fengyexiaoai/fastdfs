#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc,char* argv[])
{
    int fd[2];
    char buf[1024] = {0};
    // 1. 创建管道
    int ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe error");
        exit(0);
    }
    
    // 2.创建进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }

    if(pid == 0)
    {
        // 子进程，关闭读端
        close(fd[0]);
        
        // dup2 重定向  --后指向前
        dup2(fd[1],STDOUT_FILENO);

        // exec
        execlp("fdfs_upload_file","fdfs_upload_file","/etc/fdfs/client.conf",argv[1],NULL);
        // log
    }
    else if(pid > 0)
    {
        // 父进程，关闭写端，
        close(fd[1]);

        // wait 回收子进程
        wait(NULL);

        // read 数据从 pipe
        read(fd[0],buf,1024);
        printf("buf = %s\n",buf);
        // log
    }
    return 0;
}

/*
 * Filename: pipe.c
 */
 
#include <stdio.h>
#include <unistd.h>     //for pipe()
#include <string.h>     //for memset()
#include <stdlib.h>     //for exit()

int main()
{
    int fd[2];
    char buf[20];
    if(-1 == pipe(fd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    pid = fork();

    if(!pid){
        write(fd[1], "hello,world1", 12);
        memset(buf, '\0', sizeof(buf));
        write(fd[1], "hello,world2", 12);
        memset(buf, '\0', sizeof(buf));
        write(fd[1], "hello,world3", 12);
        memset(buf, '\0', sizeof(buf));
    }

    else if(pid>0){
        read(fd[0], buf, 12);
        printf("The message is: %s\n", buf);
        read(fd[0], buf, 12);
        printf("The message is: %s\n", buf);
        read(fd[0], buf, 12);
        printf("The message is: %s\n", buf);
    }

    else{
        perror("fork");
        exit(1);
    }

    return 0;
}
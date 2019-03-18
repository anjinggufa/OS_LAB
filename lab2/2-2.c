#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>

int main(){
	pid_t pid;
	pid = fork();
	if (pid > 0){
    	printf ("I am the parent of pid=%d!\n", pid);
    	while(1);
	}
	else if (!pid){
    	printf ("I am the baby!\n");
    	int ret;
    	ret = execl ("/usr/bin/vi", "vi","/home/anjinggufa/桌面/lab2/2-2.c", NULL);
    	if (ret == -1)
        	perror ("execl");
	}

	else if (pid == -1)
    	perror ("fork");
	return 0;
}



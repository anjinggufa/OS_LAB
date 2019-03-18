#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	pid_t pid;
	pid = fork();
	if (pid > 0){
        fork();
    	while(1){
    	    printf ("My pid is %d I am the child of pid=%d!\n",getpid(),getppid());
            sleep(10);
        }

	}
	else if (!pid){
        pid = fork();
        if(pid > 0){
			pid = fork();
    	    printf ("My pid is %d I am the child of pid=%d!\n",getpid(),getppid());
			sleep(10);
			if (pid > 0){
				int* p;
				*p = 1;	
			}
		}		
    	while(1){
    	    printf ("My pid is %d I am the child of pid=%d!\n",getpid(),getppid());
            sleep(10);
        }
	}
	else if (pid == -1)
    	perror ("fork");
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>


int main(int argc,char *argv[]){
	sem_t *mySem_1 = NULL; //控制p1、p2和p3进程打信号量
	sem_t *mySem_2 = NULL; //控制p4进程打信号量
	sem_t *mySem_3 = NULL; //控制p4进程打信号量
	mySem_1 = sem_open("mySemName_1",O_CREAT,0666,0);
	mySem_2 = sem_open("mySemName_2",O_CREAT,0666,0);
	mySem_3 = sem_open("mySemName_3",O_CREAT,0666,0);
	pid_t pid_1,pid_2,pid_3,pid_4;
	pid_1 = fork();
	if(pid_1 == 0){
		pid_2 = fork();
		if(pid_2 == 0){
			pid_3 = fork();
			if(pid_3 == 0){
				pid_4 = fork();
				if(pid_4 == 0){
					sem_wait(mySem_2);
					sem_wait(mySem_3);
					printf("I an the process P4\n");
					sem_post(mySem_2);
					sem_post(mySem_3);				
				}
				else if(pid_4>0){
					sem_wait(mySem_1);
					printf("I an the process P3\n");
					sem_post(mySem_1);	
					sem_post(mySem_3);	
				}		
			}
			else if(pid_3>0){
				sem_wait(mySem_1);
				printf("I an the process P2\n");
				sem_post(mySem_1);	
				sem_post(mySem_2);			
			}	
		}
		else if(pid_2>0){
			printf("I an the process P1\n");
			sem_post(mySem_1);
		}
	}
	sem_close(mySem_1);
	sem_close(mySem_2);
	sem_close(mySem_3);
	unlink("mySemName_1");
	unlink("mySemName_2");
	unlink("mySemName_3");
	return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

char buf[10];

sem_t *full = NULL;
sem_t *empty = NULL;

void *scan(void *arg){
	int i = 0;
	while(1){
		sem_wait(full);
		scanf("%c",&buf[(i++)%10]);
		sem_post(empty);
	}
}

void *print(void *arg){
	int i = 0;
	while(1){
		sem_wait(empty);
		printf("输出buf[%d]：%c\n",i%10,buf[i%10]);
		i++;
		sem_post(full);
		sleep(1);
	}
	return NULL;
}

int main(int argc,char *argv[]){
	pthread_t p1,p2;
	full = sem_open("full",O_CREAT,0666,10);
	empty = sem_open("empty",O_CREAT,0666,0);
	pthread_create(&p1,NULL,scan,NULL);
	pthread_create(&p2,NULL,print,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	sem_close(full);
	sem_close(empty);
	unlink("full");
	unlink("empty");
	return 0;
}



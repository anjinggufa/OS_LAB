#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

volatile int ticketCount = 1000;
sem_t *signal = NULL;

void *sell(void *arg){
	int temp;
	for(int i = 0;i < atoi(arg);i++){
		//sem_wait(signal);
		temp = ticketCount;
		//pthread_yield();
		temp = temp - 1;
		//pthread_yield();
		ticketCount = temp;
		//sem_post(signal);
	}
	return NULL;
}

void *refund(void *arg){
	int temp;
	for(int i = 0;i < atoi(arg);i++){
		//sem_wait(signal);
		temp = ticketCount;
		//pthread_yield();
		temp = temp + 1;
		//pthread_yield();
		ticketCount = temp;
		//sem_post(signal);
	}
	return NULL;
}

int main(int argc,char *argv[]){
	pthread_t p1,p2;
	if(argc!=3){
		printf("3-2<sell_num refund_num>\n");
		exit(1);
	}
	signal = sem_open("signal",O_CREAT,0666,1);
	pthread_create(&p1,NULL,sell,argv[1]);
	pthread_create(&p2,NULL,refund,argv[2]);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	printf("余票数为：%d\n",ticketCount);
	sem_close(signal);
	return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

char buf[10];

void *scan(void *arg){
	int i = 0;
	while(1){
		scanf("%c",&buf[(i++)%10]);
	}
}

void *print(void *arg){
	int i = 0;
	while(1){
		printf("输出buf[%d]：%c\n",i%10,buf[i%10]);
        i++;
		sleep(1);
	}
	return NULL;
}

int main(int argc,char *argv[]){
	pthread_t p1,p2;
	pthread_create(&p1,NULL,scan,NULL);
	pthread_create(&p2,NULL,print,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	return 0;
}
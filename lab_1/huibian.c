#include <stdio.h>
#include <unistd.h>

int main(){
	pid_t pid;
	asm volatile(
		"mov $0,%%ebx\n\t"
		"mov $0*14,%%eax\n\t"
		"int $0*80\n\t"
		"mov %%eax,%0\n\t"
		:"=m"(pid)
		);
	printf("%d\n",pid);
	return 0;
}

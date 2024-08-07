#include "mysyscall.h"
#include <stdio.h>


int main() {

	printf("Invocazione mia Syscall\n");
	long ret = mysyscall(5);
	
	printf("Risultato invocazione: %ld\n",ret);
	return 1;
	
}

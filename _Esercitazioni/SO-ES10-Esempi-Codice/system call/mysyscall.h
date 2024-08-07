#include <unistd.h>
#include <sys/syscall.h>


#define __NR_mysyscall 338

long mysyscall(int val) {

	return	syscall(__NR_mysyscall, val);
	
}

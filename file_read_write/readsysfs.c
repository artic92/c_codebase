#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define print_error \
    do { \
        fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
        __LINE__, __FILE__, errno, strerror(errno)); exit(1); \
    } while(0)

int main(int argc, char **argv)
{
    char buf[1024];
    ssize_t len;
    int fd;
    char *filename;
    char *option;

    if(argc < 2) {
        fprintf(stderr, "\nUsage:\t%s { sysfs file }\n", argv[0]);
        exit(1);
    }

    filename = argv[1]; /* accept argument 1 as filename */

    fd = open(filename, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open %s: %s\n", filename, strerror(errno));
        print_error;
    }

    len = read(fd, buf, sizeof(buf)-1);
    if (len < 0) {
        fprintf(stderr, "read %s: %s", filename, strerror(errno));
        print_error;
    }

    option = argv[2]; /* accept argument 2 as a generic command (e.g. -w) */
    if(strncmp(option, "-w", 2) == 0){
        strcpy(buf, argv[3]); /* accept argument 3 as value to write */

        len = write(fd, buf, sizeof(argv[3])-1);
        if (len < 0) {
            fprintf(stderr, "write %s: %s", filename, strerror(errno));
            print_error;
        }
    }

    buf[len] = 0;

    printf("%s : %s\n", filename, buf);

#if 0
    printf("bufferlen is %d\n", strlen(buf));
#endif

    close(fd);
    return 0;
}

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
    ssize_t len;
    int fd;
    char *filename;
    int valtowrite = -1;
    int valtoread = -1;

    if(argc < 2) {
        fprintf(stderr, "\nUsage:\t%s { file } [-w <value>]\n", argv[0]);
        exit(1);
    }

    filename = argv[1]; /* accept argument 1 as filename */

    fd = open(filename, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open %s: %s\n", filename, strerror(errno));
        print_error;
        exit(EXIT_FAILURE);
    }

    if( argv[2] != NULL ){
        if(strncmp(argv[2], "-w", 2) == 0){ /* accept argument 2 as a write command */

            valtowrite = atoi(argv[3]);

            printf("[%s] value to write: %i\n", __func__, valtowrite);

            len = write(fd, &valtowrite, sizeof(valtowrite));
            if (len < 0) {
                fprintf(stderr, "write %s: %s", filename, strerror(errno));
                print_error;
                exit(EXIT_FAILURE);
            }
        }
    }

    len = read(fd, &valtoread, sizeof(int));
    if (len < 0) {
        fprintf(stderr, "read %s: %s", filename, strerror(errno));
        print_error;
        exit(EXIT_FAILURE);
    }
    printf("value read: `%d`\n", valtoread);

    close(fd);
    return 0;
}

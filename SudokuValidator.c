#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
    unsigned char *f;
    int size = 81;
    const char * file_name = argv[1];
    int fd = open (argv[1], O_RDONLY);
    int i;
    f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    for (i = 0; i < size; i++) {
    	int now = (int)f[i] - '0';
        printf("%d\n", now);
    }

    return 0;
}
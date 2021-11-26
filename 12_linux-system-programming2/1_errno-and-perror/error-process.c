#include <stdio.h>
#include <sys/file.h>
#include <errno.h>
int main()
{
        int fd;
        fd = open("nonsexist.txt", O_RDONLY);
        if (fd==-1)
        {
                printf("errno = %d\n", errno);
                perror("main");
        }
        fd = open("/", O_WRONLY);
        if (fd==-1)
        {
                printf("errno = %d\n", errno);
                perror("main");
        }
        fd = open("nonsexist.txt", O_RDONLY| O_CREAT, 0644);
        printf("errno = %d\n", errno);
        perror("main");
        errno = 0;
        perror("main");
        return 0;
}
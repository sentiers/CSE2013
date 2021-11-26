#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
        int i, fd;
        int charsRead;
        char buffer[4096];
        fd = open("normal.txt", O_CREAT| O_RDWR, 0600);
        write(fd, "normal", 6); /* write(int fd, char *buf, int count) */
        for (i=1;i<=70;i++)
                write(fd, "/0", 1);
        write(fd,"file",4);
        close(fd);
        fd = open("normal.txt", O_RDONLY);
        charsRead = read(fd, buffer,100); /* read(int fd, char *buf, int count) */
        write(1, buffer, charsRead);
        close(fd);
        return 0;
}
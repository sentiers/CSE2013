#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
        int i, fd1, fd2;
        int charsRead;
        char buffer[4096];
        fd1 = open("SRCFILE", O_RDONLY);
        fd2 = open("COPYFILE", O_CREAT|O_RDWR, 0666);
        charsRead = read(fd1, buffer, 2000);
        write(fd2, buffer, charsRead);
        close(fd1);
        close(fd2);
        return 0;
}
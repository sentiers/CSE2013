#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
void sig_handler(int num){
        printf("\nChild Sent a signal to parent:%d\n",num);
        signal(SIGALRM,SIG_DFL);
}
int main()
{
        int status;
        system("clear");
        printf("\n--------Signal Handling Across Processes----------\n");
        switch(fork())
        {
                case -1:
                        perror("\nFork Failed...\n");
                        exit(1);
                        break;
                case 0:
                        alarm(3);
                        kill(getppid(),SIGALRM);
                        // signal(SIGALRM,sig_handler);
                        printf("\nIts Child Process\n");
                        sleep(5);
                        break;
                default:
                        signal(SIGALRM,sig_handler);
                        wait(&status);
                        printf("\nIts Parent Process\n");
        }
}

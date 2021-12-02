#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int status;

    pid = fork();
    if(pid > 0) {               /* 부모 프로세스 */
        printf("parent: waiting..\n");
        wait(&status);
        // 전달받은 210이 부모프로세스의 두번째 바이트에 저장
        // 따라서 210*256 = 53760이 출력되게 된다.
        printf("parent: status is %d\n", status);
    }
    else if(pid == 0) { /* 자식 프로세스 */
        sleep(1);
        printf("child: bye!\n");
        exit(1234);
        // 1234의 하위 8bit, 즉,
        // 모듈러 연산 1234%256 = 210이 전달
    }
    else
        printf("fail to fork\n");

    printf("bye!\n");
    return 0;
}

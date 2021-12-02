#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main()
{
    char *originalname = "test.txt";
    char *hardfilename = "test.txt.hard";
    char *softfilename = "test.txt.soft";

    int filedes, retval;
    mode_t oldmask; // 파일모드를 저장할 수 있는  데이터 타입
    char buffer[1024];
    int  nread;
    struct stat finfo; // stat 구조체는 헤더파일 <sys/types.h>에 존재
    oldmask = umask(0377);
    // 소유자는 쓰기/실행이 가능하고 다른 모든 사람은 모든 권한

    filedes = open(originalname, O_RDWR | O_CREAT, 0755);
    // 소유자는 읽기/쓰기/실행 권한을 가지고 나머지 모든 사람은 읽기/실행 권한
    close(filedes);

    // 권한 확인
   if((retval = access(originalname, W_OK)) == -1)  {
        printf("%s is not writable\n", originalname);
        chmod(originalname, 0644);
        // 소유자는 읽기/쓰기가 가능하고 나머지 모든 사람은 읽기만 가능
 }
     link(originalname, hardfilename); // 하드링크 생성
     symlink(originalname, softfilename); // 심볼릭 링크 생성

     rename(hardfilename, "newname.txt"); // 파일명 변경
     nread = readlink(softfilename, buffer, 512); // 원본파일명 가져오기
     write(1, buffer, nread); // 원본파일명 출력

     stat(originalname, &finfo); // 원본파일 정보 stat 구조체에 저장
     printf("\n%s\n", originalname);
     printf("File mode   : %o\n", finfo.st_mode); // 파일모드
     printf("Files size  : %d\n", finfo.st_size); // 파일사이즈
     printf("Num of blocks  : %d\n", finfo.st_blocks); // 512B 블록 수
   return 0;
}

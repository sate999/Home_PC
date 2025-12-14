/* ============================================================
 * pipeclient.c - 연습문제 2: Named Pipe 클라이언트 (주석 있는 버전)
 * 
 * 문제:
 * pipeclient.c:
 * - The client writes an argv[1] file contents to a named pipe.
 * - pipeclient는 argv[1] 파일의 내용을 named pipe에 write하고 종료한다.
 * 
 * 컴파일: gcc -o pipeclient pipeclient.c
 * 실행: ./pipeclient <파일이름>
 * 예시: ./pipeclient data
 * 
 * 동작:
 * 1. argv[1] 파일 열기
 * 2. Public named pipe 열기 (쓰기 전용)
 * 3. 파일 내용을 읽어서 named pipe에 write
 * 4. 종료
 * ============================================================ */

#include <fcntl.h>          /* open, O_RDONLY, O_WRONLY */
#include <sys/types.h>      /* 타입 정의 */
#include <sys/stat.h>       /* 파일 상태 */
#include <stdlib.h>         /* exit */
#include <stdio.h>          /* printf, perror */
#include <unistd.h>         /* read, write, close */

#define PUBLIC "Public"     /* 공개 named pipe 이름 (서버와 동일) */
#define LINESIZE 512        /* 버퍼 크기 */

int main (int argc, char *argv[]) {
    int fdpub, fd, n;
    char line[LINESIZE];
    
    /* 인자 검사 */
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    
    /* --------------------------------------------------------
     * argv[1] 파일 열기 (읽기 전용)
     * - 서버에게 보낼 파일
     * -------------------------------------------------------- */
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(1);
    }
    
    /* --------------------------------------------------------
     * Public Named Pipe 열기 (쓰기 전용)
     * - 서버가 읽기 전용으로 열어놓은 파이프
     * - 서버가 아직 안 열었으면 blocking
     * -------------------------------------------------------- */
    if ((fdpub = open(PUBLIC, O_WRONLY)) == -1) {
        perror(PUBLIC);
        exit(2);
    }
    
    /* --------------------------------------------------------
     * 파일 내용을 읽어서 Named Pipe에 write
     * - 서버가 이 데이터를 읽어서 화면에 출력
     * -------------------------------------------------------- */
    while ((n = read(fd, line, LINESIZE)) > 0) {
        write(fdpub, line, n);
    }
    
    /* --------------------------------------------------------
     * 정리 작업
     * - 파일과 파이프 닫기
     * -------------------------------------------------------- */
    close(fd);
    close(fdpub);
    
    exit(0);
}
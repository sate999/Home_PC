/* ============================================================
 * pipeserver.c - 연습문제 2: Named Pipe 서버 (주석 있는 버전)
 * 
 * 문제:
 * pipeserver.c:
 * - The program reads what is written to a named pipe and 
 *   writes it to standard output.
 * - pipeserver가 초기화 과정에서 생성한 named pipe 이름은 
 *   public하게 알려져 있다.
 * - pipeserver는 종료하지 않는 daemon process 형식으로 동작한다.
 * 
 * 컴파일: gcc -o pipeserver pipeserver.c
 * 실행: ./pipeserver (백그라운드에서 실행: ./pipeserver &)
 * 
 * 동작:
 * 1. Public named pipe 생성
 * 2. 클라이언트가 보낸 데이터를 읽어서 화면에 출력
 * 3. 무한 반복 (daemon)
 * ============================================================ */

#include <fcntl.h>          /* open, O_RDONLY */
#include <sys/types.h>      /* 타입 정의 */
#include <sys/stat.h>       /* S_IFIFO, mknod */
#include <stdlib.h>         /* exit */
#include <stdio.h>          /* printf, perror */
#include <unistd.h>         /* read, write, close */

#define PUBLIC "Public"     /* 공개 named pipe 이름 */
#define LINESIZE 512        /* 버퍼 크기 */

int main (int argc, char *argv[]) {
    int fdpub, n;
    char line[LINESIZE];
    
    /* --------------------------------------------------------
     * Public Named Pipe (FIFO) 생성
     * - mknod로 FIFO 특수 파일 생성
     * - S_IFIFO: FIFO 파일 타입
     * - 0666: 읽기/쓰기 권한
     * - 이미 존재하면 에러 발생하지만 무시
     * -------------------------------------------------------- */
    mknod (PUBLIC, S_IFIFO | 0666, 0);
    
    /* --------------------------------------------------------
     * Named Pipe를 읽기 전용으로 열기
     * - 클라이언트가 쓰기 전용으로 열어서 데이터를 보냄
     * -------------------------------------------------------- */
    if ((fdpub = open(PUBLIC, O_RDONLY)) == -1) {
        perror(PUBLIC);
        exit(1);
    }
    
    printf("Server started. Waiting for clients...\n");

loop: /* 무한 루프 - daemon process */
    
    /* --------------------------------------------------------
     * Named Pipe에서 데이터 읽기
     * - 클라이언트가 write할 때까지 blocking
     * - 읽은 데이터를 stdout에 출력
     * -------------------------------------------------------- */
    while ((n = read(fdpub, line, LINESIZE)) > 0) {
        write(1, line, n);      /* stdout(1)에 출력 */
    }
    
    /* --------------------------------------------------------
     * read가 0 반환 (EOF)
     * - 모든 writer가 close했을 때
     * - 파이프를 다시 열어서 다음 클라이언트 대기
     * -------------------------------------------------------- */
    close(fdpub);
    
    if ((fdpub = open(PUBLIC, O_RDONLY)) == -1) {
        perror(PUBLIC);
        exit(1);
    }
    
    goto loop;  /* 무한 반복 (다음 클라이언트 대기) */
}
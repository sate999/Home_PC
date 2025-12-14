/* ============================================================
 * server.c - Named Pipe 파일 서버 (주석 있는 버전)
 * 
 * 기능: 클라이언트가 요청한 파일의 내용을 개인 파이프로 전송
 * 컴파일: gcc -o server server.c
 * 실행: ./server (백그라운드에서 실행 권장: ./server &)
 * 
 * 동작 순서:
 * 1. Public FIFO 파이프 생성
 * 2. 클라이언트 요청 대기 (blocking)
 * 3. 요청 수신 시 파일 열기
 * 4. 클라이언트의 개인 FIFO로 파일 내용 전송
 * 5. 다시 2번으로 (무한 반복)
 * ============================================================ */

#include "namedPipe.h"

int main (int argc, char *argv[]) {
    int fdpub, fdpriv, fd, n, i;
    struct message msg;
    char line[LINESIZE];
    
    /* --------------------------------------------------------
     * Public FIFO 생성
     * - mknod로 named pipe 파일 생성
     * - S_IFIFO: FIFO 특수 파일 타입
     * - 0666: 읽기/쓰기 권한
     * - 이미 존재하면 에러 발생하지만 무시 (이미 있으면 그냥 사용)
     * -------------------------------------------------------- */
    mknod (PUBLIC, S_IFIFO | 0666, 0);
    
    /* Public FIFO를 읽기 전용으로 열기 */
    if ((fdpub = open(PUBLIC, O_RDONLY)) == -1) {
        perror(PUBLIC);
        exit(1);
    }

loop: /* 무한 루프 - 데몬 프로세스처럼 동작 */
    
    /* --------------------------------------------------------
     * 클라이언트 요청 대기
     * - Public 파이프에서 message 구조체 읽기
     * - 클라이언트가 write할 때까지 blocking
     * -------------------------------------------------------- */
    while (read (fdpub, (char *) &msg, sizeof(msg)) > 0) {
        printf ("Client Request Arrived.\n");
        
        /* --------------------------------------------------------
         * 클라이언트가 요청한 파일 열기
         * - msg.filename: 클라이언트가 읽기를 원하는 파일
         * -------------------------------------------------------- */
        if ((fd=open (msg.filename, O_RDONLY)) == -1) {
            perror(msg.filename);
            break;
        }
        
        /* --------------------------------------------------------
         * 클라이언트의 개인 FIFO 열기 (재시도 포함)
         * - msg.privfifo: 클라이언트가 생성한 개인 파이프 이름
         * - O_NDELAY: non-blocking 모드
         * - 클라이언트가 mknod로 생성 중일 수 있어서 재시도
         * -------------------------------------------------------- */
        for (i=0; i< NUMTRIES; i++) {
            if ((fdpriv = open (msg.privfifo, O_WRONLY | O_NDELAY)) == -1) 
                sleep(1);       /* 1초 대기 후 재시도 */
            else 
                break;          /* 성공하면 루프 탈출 */
        }
        
        if (fdpriv == -1) {
            perror (msg.privfifo);
            break;
        }
        
        /* --------------------------------------------------------
         * 파일 내용을 개인 FIFO로 전송
         * - 파일에서 읽어서 클라이언트 파이프에 쓰기
         * -------------------------------------------------------- */
        while ((n=read (fd, line, LINESIZE)) > 0)
            write (fdpriv, line, n);
        
        /* 파일과 개인 파이프 닫기 */
        close (fd);
        close (fdpriv);
    }
    
    goto loop;  /* 무한 반복 (다음 클라이언트 대기) */
}
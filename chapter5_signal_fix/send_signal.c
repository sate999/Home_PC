/* ============================================================
 * send_signal.c - 시그널 전송 부모 프로그램 (주석 있는 버전)
 * 
 * 기능: 자식 프로세스(rcv_signal) 실행 후 SIGINT 시그널 전송
 *       자식 종료 상태(status) 분석
 * 컴파일: gcc -o send_signal send_signal.c
 * 사전 준비: gcc -o rcv_signal rcv_signal.c
 *            export PATH=$PATH:.
 * 실행: ./send_signal
 * 
 * 출력 예시:
 * [Child] sleep count #1
 * [Child] sleep count #2
 * [Parent]: Send SIGINT
 * [Child] receive SIGINT
 * [Parent]: Child killed by Parent. [1900]
 * 
 * status 분석:
 * - 자식의 자의적 종료: 상위 바이트에 exit code
 *   예: exit(25) → status = 0x1900
 * - 시그널에 의한 종료: 하위 바이트에 signal number
 *   예: SIGINT(2) → status = 0x0002
 * ============================================================ */

#include <stdio.h>          /* printf 함수 */
#include <sys/wait.h>       /* wait 함수 */
#include <signal.h>         /* kill 함수, SIGINT */
#include <unistd.h>         /* fork, sleep, execlp 함수 */
#include <stdlib.h>         /* exit 함수 */

int main() {
    int pid = 0, status = 0;
    
    if ((pid = fork()) == 0) {          /* 자식 프로세스 */
        /* --------------------------------------------------------
         * execlp로 rcv_signal 프로그램 실행
         * - PATH에서 rcv_signal 검색하여 실행
         * - 현재 디렉토리가 PATH에 있어야 함
         * -------------------------------------------------------- */
        execlp ("rcv_signal", "rcv_signal", (char *)NULL);
        perror ("execlp() : ");
        exit(2);
    }
    else if (pid > 0) {                 /* 부모 프로세스 */
        sleep (2);                      /* 2초 대기 */
        
        /* --------------------------------------------------------
         * kill()로 자식에게 SIGINT 전송
         * - 자식의 핸들러가 호출됨
         * -------------------------------------------------------- */
        kill (pid, SIGINT);
        printf ("[Parent]: Send SIGINT\n");
        
        /* --------------------------------------------------------
         * wait()로 자식 종료 대기 및 상태 수신
         * - status: 자식의 종료 상태
         * -------------------------------------------------------- */
        wait (&status);
        printf ("[Parent]: Child killed by Parent. [%x] \n", status);
    }
    
    return 0;
}
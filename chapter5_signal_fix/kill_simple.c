/* ============================================================
 * kill_simple.c - 간단한 kill() 시스템 콜 예제 (주석 있는 버전)
 * 
 * 기능: 부모가 자식에게 SIGQUIT 시그널을 전송
 * 컴파일: gcc -o kill_simple kill_simple.c
 * 실행: ./kill_simple
 * 
 * 출력 예시:
 * Child: signal [3] occurred.
 * ============================================================ */

#include <signal.h>         /* signal, kill 함수, SIGQUIT */
#include <stdio.h>          /* printf 함수 */
#include <unistd.h>         /* fork, sleep 함수 */

/* 자식 프로세스용 시그널 핸들러 */
void child (int sig) {
    printf ("Child: signal [%d] occurred.\n", sig);
}

int main () {
    int childPid;
    
    if ((childPid = fork()) == 0) {     /* 자식 프로세스 */
        signal (SIGQUIT, child);        /* SIGQUIT = 3 */
        while (1) {
            sleep (1);
        }
    }
    
    /* 부모 프로세스 */
    sleep (2);  /* 2초 대기 (자식이 핸들러 등록할 시간) */
    
    /* --------------------------------------------------------
     * kill() 시스템 콜
     * - 특정 프로세스에게 시그널 전송
     * - kill(pid, signum): pid 프로세스에게 signum 시그널 전송
     * -------------------------------------------------------- */
    kill (childPid, SIGQUIT);   /* 자식에게 SIGQUIT 전송 */
    
    return 0;
}
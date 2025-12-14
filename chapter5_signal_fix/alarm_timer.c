/* ============================================================
 * alarm_timer.c - 알람 타이머 부모 프로그램 (주석 있는 버전)
 * 
 * 기능: 자식 프로세스 실행 후 타임아웃(10초) 설정
 *       타임아웃 시 SIGKILL로 자식 강제 종료
 * 컴파일: gcc -o alarm_timer alarm_timer.c
 * 사전 준비: gcc -o rcv_signal_2 rcv_signal_2.c
 * 실행: ./alarm_timer rcv_signal_2
 * 
 * 시나리오:
 * 1. rand_num < 10: 자식이 먼저 종료 → status = 0x3500
 * 2. rand_num >= 10: 타임아웃 → SIGKILL로 종료 → status = 0x0009
 * ============================================================ */

#include <signal.h>         /* signal, kill, alarm 함수, SIGALRM, SIGKILL */
#include <errno.h>          /* errno */
#include <stdio.h>          /* printf 함수 */
#include <stdlib.h>         /* exit 함수 */
#include <unistd.h>         /* fork, execvp 함수 */
#include <sys/wait.h>       /* wait 함수 */

#define TIMEOUT 10          /* 타임아웃 시간 (초) */

int pid;    /* 전역 변수: 자식 PID (핸들러에서 사용) */

/* ============================================================
 * sigalarm - SIGALRM 시그널 핸들러
 * 
 * 동작:
 * - 타임아웃 메시지 출력
 * - 자식 프로세스에게 SIGKILL 전송 (강제 종료)
 * ============================================================ */
void sigalarm (int sig) {
    printf ("SIGALRM occurred.\n");
    kill (pid, SIGKILL);    /* 자식 강제 종료 */
} 

int main (int argc, char *argv[]) {
    int status;
    
    if ((pid = fork()) == 0) {          /* 자식 프로세스 */
        /* --------------------------------------------------------
         * execvp로 argv[1] 프로그램 실행
         * - argv[1]: 실행할 프로그램 이름
         * - &argv[1]: 인자 배열
         * -------------------------------------------------------- */
        execvp(argv[1], &argv[1]);
        perror(argv[1]);
        exit(127);
    }
    
    /* 부모 프로세스 */
    
    /* SIGALRM 핸들러 등록 */
    signal (SIGALRM, sigalarm);
    
    /* --------------------------------------------------------
     * alarm(TIMEOUT): TIMEOUT초 후 SIGALRM 발생
     * - 자식이 먼저 종료하면 알람 취소 필요
     * -------------------------------------------------------- */
    alarm (TIMEOUT);
    
    /* 자식 종료 대기 */
    wait (&status);
    
    /* --------------------------------------------------------
     * alarm(0): 남은 알람 시간 반환 및 알람 취소
     * - 자식이 타임아웃 전에 종료하면 남은 시간 > 0
     * - 타임아웃이 발생했으면 남은 시간 = 0
     * -------------------------------------------------------- */
    printf ("Parent: time remaining: %d \n", alarm(0));
    printf ("Parent: child exit status value =[%x]\n", status);
    
    return 0;
}
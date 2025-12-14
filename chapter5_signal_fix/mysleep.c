/* ============================================================
 * mysleep.c - 사용자 정의 sleep 함수 예제 (주석 있는 버전)
 * 
 * 기능: alarm()과 pause()를 사용한 sleep 구현
 *       다른 시그널에 의해 깨어나면 남은 시간 반환
 * 컴파일: gcc -o mysleep mysleep.c
 * 실행: ./mysleep 5
 * 
 * 출력 예시 (5초 경과):
 * Receive SIGALRM signal
 * remaining time : 0
 * 
 * 출력 예시 (1초 후 ^C):
 * Receive SIGINT signal
 * remaining time : 4
 * ============================================================ */

#include <stdio.h>          /* printf 함수 */
#include <signal.h>         /* signal, alarm 함수 */
#include <setjmp.h>         /* (예제에서 include하지만 미사용) */
#include <stdlib.h>         /* atoi, exit 함수 */
#include <unistd.h>         /* pause 함수 */

void sigint_handler(int);
void sigalrm_handler(int);
int mysleep (int);

int main (int argc, char *argv[]) {
    int ret = 0;
    
    /* SIGINT 핸들러 등록 (^C로 깨울 수 있도록) */
    signal (SIGINT, sigint_handler);
    
    /* mysleep 호출 (argv[1]초 동안 대기) */
    ret = mysleep (atoi(argv[1]));
    
    printf ("remaining time : %d\n", ret);
    exit(0);
}

/* SIGINT 핸들러 */
void sigint_handler (int sig) {
    if (sig == SIGINT)
        printf ("Receive SIGINT signal\n");
}

/* ============================================================
 * mysleep - 사용자 정의 sleep 함수
 * 
 * Parameter:
 *   sec - sleep할 시간 (초)
 * 
 * Return:
 *   남은 시간 (다른 시그널에 의해 깨어난 경우)
 * 
 * 동작:
 * 1. SIGALRM 핸들러 등록 (이전 핸들러 저장)
 * 2. alarm(sec) 설정
 * 3. pause()로 시그널 대기
 * 4. 남은 알람 시간 확인 및 알람 취소
 * 5. 이전 핸들러 복원
 * ============================================================ */
int mysleep (int sec) {
    int remain_sec = 0;
    void *old_set;
    
    /* SIGALRM 핸들러 등록 및 이전 핸들러 저장 */
    old_set = signal(SIGALRM, sigalrm_handler);
    
    /* sec초 후 SIGALRM 발생하도록 설정 */
    alarm(sec);
    
    /* --------------------------------------------------------
     * pause(): 시그널 도착까지 대기
     * 다음 경우에 깨어남:
     * 1. SIGALRM 발생 (sec초 경과)
     * 2. SIGINT 발생 (사용자가 ^C)
     * 3. 기타 시그널 발생
     * -------------------------------------------------------- */
    pause();
    
    /* --------------------------------------------------------
     * alarm(0): 이전 알람 취소, 남은 시간 반환
     * - SIGALRM에 의해 깨어났으면: 0 반환
     * - 다른 시그널에 의해 깨어났으면: 남은 시간 반환
     * -------------------------------------------------------- */
    remain_sec = alarm(0);
    
    /* 이전 SIGALRM 핸들러 복원 */
    signal (SIGALRM, old_set);
    
    return remain_sec;
}

/* SIGALRM 핸들러 */
void sigalrm_handler (int sig) {
    printf ("Receive SIGALRM signal\n");
}
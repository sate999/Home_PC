/* ============================================================
 * rcv_signal.c - 시그널 수신 자식 프로그램 (주석 있는 버전)
 * 
 * 기능: 1초마다 카운트 출력, SIGINT 수신 시 핸들러 실행 후 종료
 *       send_signal 프로그램과 함께 사용
 * 컴파일: gcc -o rcv_signal rcv_signal.c
 * 실행: (send_signal에서 execlp로 실행됨)
 * 
 * 출력 예시:
 * [Child] sleep count #1
 * [Child] sleep count #2
 * [Child] receive SIGINT
 * ============================================================ */

#include <stdio.h>          /* printf 함수 */
#include <signal.h>         /* signal 함수, SIGINT */
#include <stdlib.h>         /* exit 함수 */
#include <unistd.h>         /* sleep 함수 */
#include <sys/types.h>      /* 타입 정의 */

#define MAX_COUNT 10        /* 최대 카운트 수 */

void sigint_signalhandler (int);

int main() {
    int i = 0;
    
    /* SIGINT 핸들러 등록 */
    signal (SIGINT, sigint_signalhandler);
    
    /* 1초마다 카운트 출력 */
    for (i = 1; i < MAX_COUNT; i++) {
        printf ("\t[Child] sleep count #%d\n", i);
        sleep (1);
    }
    
    printf ("\t[Child] process terminate! \n");
    exit (0);   /* 정상 종료 */
}

/* ============================================================
 * sigint_signalhandler - SIGINT 시그널 핸들러
 * 
 * 동작:
 * - SIGINT 수신 메시지 출력
 * - exit(25)로 종료 (exit code = 0x19)
 * - wait()로 확인 시 status = 0x1900
 * ============================================================ */
void sigint_signalhandler (int sig) {
    printf("\t[Child] receive SIGINT\n");
    exit (25);  /* 0x19 - 상위 바이트에 저장됨 */
}
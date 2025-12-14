/* ============================================================
 * rcv_signal_2.c - 랜덤 sleep 자식 프로그램 (주석 있는 버전)
 * 
 * 기능: 0~19 사이의 랜덤 숫자만큼 sleep 후 종료
 *       alarm_timer와 함께 사용하여 타임아웃 테스트
 * 컴파일: gcc -o rcv_signal_2 rcv_signal_2.c
 * 실행: (alarm_timer에서 실행됨)
 * 
 * 동작:
 * - rand_num < 10: 타임아웃 전에 정상 종료
 * - rand_num >= 10: 타임아웃으로 SIGKILL에 의해 종료
 * ============================================================ */

#include <stdio.h>          /* printf 함수 */
#include <signal.h>         /* 시그널 관련 */
#include <stdlib.h>         /* rand, srand, exit 함수 */
#include <sys/types.h>      /* 타입 정의 */
#include <unistd.h>         /* getpid, sleep 함수 */

#define LIMIT_TIME 20       /* 랜덤 숫자 최대값 */

int main() {
    int rand_num;
    
    /* --------------------------------------------------------
     * 난수 생성
     * - srand(seed): 난수 시드 설정 (PID 사용)
     * - rand(): 0 ~ RAND_MAX(2^31-1) 사이 난수 생성
     * -------------------------------------------------------- */
    srand (getpid());                   /* PID를 시드로 설정 */
    rand_num = rand();                  /* 난수 생성 */
    rand_num %= LIMIT_TIME;             /* 0~19 범위로 제한 */
    
    printf ("\t[Child] : sleep random number : %d\n", rand_num);
    
    /* rand_num초 동안 sleep */
    sleep (rand_num);
    
    printf ("\t[Child] : process terminate!\n");
    exit (0x35);    /* exit code = 53 (0x35) */
}
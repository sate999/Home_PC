/* ============================================================
 * pause_example.c - pause() 함수 예제 (주석 있는 버전)
 * 
 * 기능: pause()로 시그널 대기, 핸들러 실행 후 계속 진행
 * 컴파일: gcc -o pause_example pause_example.c
 * 실행: ./pause_example
 * 
 * 출력 예시:
 * in handler.
 * i=[-1]
 * 
 * pause() 동작:
 * - 시그널이 올 때까지 프로세스 정지
 * - 핸들러가 있으면 핸들러 실행 후 pause() 반환 (-1 반환)
 * - 핸들러가 없으면 기본 동작 수행 (대부분 종료)
 * ============================================================ */

#include <stdio.h>          /* fprintf 함수 */
#include <signal.h>         /* signal, alarm 함수, SIGALRM */
#include <unistd.h>         /* pause 함수 */

void func (int signum);

int main () {
    int i;
    
    /* SIGALRM 핸들러 등록 */
    signal (SIGALRM, func);
    
    /* --------------------------------------------------------
     * alarm(1): 1초 후 SIGALRM 발생
     * pause(): 시그널 도착까지 대기
     * - 1초 후 SIGALRM 발생 → func() 실행 → pause() 반환
     * -------------------------------------------------------- */
    alarm (1);
    i = pause();    /* 시그널 대기 (핸들러 실행 후 -1 반환) */
    
    fprintf (stderr, "i=[%d]\n", i);
    
    return 0;
}

/* SIGALRM 핸들러 */
void func (int signum) {
    fprintf (stderr, "in handler.\n");
}
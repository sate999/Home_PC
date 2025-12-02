/*
 * alarm_basic.c
 * 출처: Chapter 5 Signal PDF - alarm example (41페이지)
 * 
 * 목적: alarm() 함수로 SIGALRM 시그널 발생시키기
 * 
 * 실행 방법: gcc -o alarm_basic alarm_basic.c && ./alarm_basic 3
 * (3초 후에 SIGALRM 발생)
 */

#include <stdio.h>      // printf() 함수 사용
#include <stdlib.h>     // exit(), atoi() 함수 사용
#include <unistd.h>     // alarm() 함수 사용
#include <signal.h>     // signal(), SIGALRM 사용

/*
 * SIGALRM 시그널 핸들러 함수
 * 
 * alarm()으로 설정한 시간이 지나면 이 함수가 호출됨
 */
void fun(int num) {
    printf("SIGALARM signal.\n");   // 알람 발생 메시지 출력
    exit(1);                         // 프로그램 종료
}

int main(int argc, char *argv[]) {
    
    /*
     * signal(SIGALRM, fun);
     * 
     * SIGALRM(14번) 시그널이 발생하면 fun() 함수를 실행하도록 등록
     * 
     * 만약 이 줄이 없으면?
     * → SIGALRM의 기본 동작(프로세스 종료)이 수행됨
     */
    signal(SIGALRM, fun);
    
    /*
     * alarm(atoi(argv[1]));
     * 
     * atoi(argv[1]) = 명령행 인자를 정수로 변환
     * 예: ./alarm_basic 3 → argv[1]은 "3" → atoi("3")은 3
     * 
     * alarm(3) = 3초 후에 SIGALRM 시그널 발생하도록 예약
     */
    alarm(atoi(argv[1]));
    
    /*
     * while (1) ;
     * 
     * 무한 루프 (CPU 계속 사용)
     * alarm()으로 설정한 시간이 지나면 SIGALRM 발생
     * → fun() 핸들러 실행 → exit(1)로 종료
     */
    while (1)
        ;   // 아무것도 하지 않고 대기 (busy waiting)
    
    return 0;   // 실제로는 여기까지 도달하지 않음
}
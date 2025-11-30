/*
 * sig_change.c
 * 목적: signal() 리턴값을 이용해 핸들러 교체하기
 * 
 * 실행 방법: gcc -o sig_change sig_change.c && ./sig_change
 * 테스트: Ctrl+C를 3번 눌러보세요 (핸들러가 바뀜!)
 */

#include <stdio.h>      // printf() 함수 사용
#include <signal.h>     // signal() 함수 사용
#include <unistd.h>     // sleep() 함수 사용
#include <stdlib.h>     // exit() 함수 사용

/*
 * 첫 번째 핸들러 함수
 */
void handler1(int signum) {
    printf("\n★★★ handler1 실행! (시그널 번호: %d) ★★★\n\n", signum);
}

/*
 * 두 번째 핸들러 함수
 */
void handler2(int signum) {
    printf("\n▲▲▲ handler2 실행! (시그널 번호: %d) ▲▲▲\n\n", signum);
}

/*
 * sighandler_t 타입 설명:
 * 
 * typedef void (*sighandler_t)(int);
 * 
 * 이것은 "int를 파라미터로 받고 void를 리턴하는 함수의 포인터" 타입
 * 즉, 시그널 핸들러 함수를 가리키는 포인터
 */
typedef void (*sighandler_t)(int);

int main() {
    sighandler_t old_handler;
    int count = 0;
    
    
    printf("=== 1단계: handler1 등록 ===\n");
    signal(SIGINT, handler1);
    printf("Ctrl+C를 누르면 handler1이 실행됩니다.\n\n");
    
    
    for (count = 0; count < 5; count++) {
        printf("1단계 진행 중... %d초\n", count);
        sleep(1);
    }
    
   
    printf("\n=== 2단계: handler2로 교체 ===\n");
    old_handler = signal(SIGINT, handler2);
    printf("Ctrl+C를 누르면 handler2가 실행됩니다.\n");
    printf("(이전 핸들러 handler1은 old_handler에 저장됨)\n\n");
    
    
    for (count = 0; count < 5; count++) {
        printf("2단계 진행 중... %d초\n", count);
        sleep(1);
    }
    
   
    printf("\n=== 3단계: handler1으로 복원 ===\n");
    signal(SIGINT, old_handler);
    printf("Ctrl+C를 누르면 다시 handler1이 실행됩니다.\n\n");
    
    
    for (count = 0; count < 5; count++) {
        printf("3단계 진행 중... %d초\n", count);
        sleep(1);
    }
    
    printf("\n프로그램 정상 종료.\n");
    return 0;
}
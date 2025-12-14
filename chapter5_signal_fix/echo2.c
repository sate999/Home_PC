/* ============================================================
 * echo_2.c - 다중 시그널 핸들러 예제 (주석 있는 버전)
 * 
 * 기능: SIGINT(^C)와 SIGQUIT(^\) 두 시그널을 처리하는 핸들러 등록
 *       SIGINT 수신 시 종료, SIGQUIT 수신 시 계속 실행
 * 컴파일: gcc -o echo_2 echo_2.c
 * 실행: ./echo_2
 * 
 * 출력 예시:
 * this is
 * this is
 * test....
 * test....
 * SIGQUIT received and process continue (^\)
 * SIGQUIT received and process continue (^\)
 * test.
 * test.
 * SIGINT received and exit. (^C)
 * ============================================================ */

#include <stdio.h>          /* printf 함수 사용 */
#include <sys/types.h>      /* 타입 정의 */
#include <signal.h>         /* signal 함수, SIGINT, SIGQUIT 상수 */
#include <unistd.h>         /* read, write 함수 */
#include <stdlib.h>         /* exit 함수 */

/* 시그널 핸들러 함수 선언 */
void signalhandler (int);

int main() {
    int size = 0;
    char temp[BUFSIZ];
    
    /* --------------------------------------------------------
     * 두 개의 시그널에 동일한 핸들러 등록
     * - SIGINT (2): Ctrl+C (인터럽트)
     * - SIGQUIT (3): Ctrl+\ (종료)
     * -------------------------------------------------------- */
    signal (SIGINT, signalhandler);
    signal (SIGQUIT, signalhandler);
    
    /* 무한 루프: 표준입력에서 읽어서 표준출력으로 쓰기 */
    while (1) {
        size = read(0, temp, BUFSIZ);
        write(1, temp, size);
    }
    
    exit(0);
}

/* ============================================================
 * signalhandler - 통합 시그널 핸들러 함수
 * 
 * Parameter:
 *   sig - 발생한 시그널 번호
 * 
 * 동작:
 *   SIGINT: 메시지 출력 후 종료
 *   SIGQUIT: 메시지 출력 후 계속 실행
 * ============================================================ */
void signalhandler (int sig) {
    if (sig == SIGINT) {
        printf ("SIGINT received and exit\n");
        exit(1);    /* SIGINT는 프로그램 종료 */
    }
    else if (sig == SIGQUIT) {
        printf ("SIGQUIT received and process continue.\n");
        /* SIGQUIT는 종료하지 않고 계속 실행 */
    }
}
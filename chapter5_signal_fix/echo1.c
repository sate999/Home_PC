/* ============================================================
 * echo_1.c - 기본 시그널 핸들러 예제 (주석 있는 버전)
 * 
 * 기능: 표준입력을 읽어서 출력하고, SIGINT(^C) 수신 시 핸들러 실행
 * 컴파일: gcc -o echo_1 echo_1.c
 * 실행: ./echo_1
 * 
 * 출력 예시:
 * This is test.
 * This is test.
 * ok
 * ok  ^C (SIGINT)
 * SIGINT Received. [2]
 * ============================================================ */

#include <stdio.h>          /* printf 함수 사용 */
#include <sys/types.h>      /* 타입 정의 */
#include <signal.h>         /* signal 함수, SIGINT 상수 */
#include <stdlib.h>         /* exit 함수 */
#include <unistd.h>         /* read, write 함수 */

/* 시그널 핸들러 함수 선언 */
void sigint_signalhandler (int);

int main() {
    int size = 0;           /* read로 읽은 바이트 수 */
    char temp[BUFSIZ];      /* 입력 버퍼 (BUFSIZ는 stdio.h에 정의됨) */
    
    /* --------------------------------------------------------
     * signal() 함수로 SIGINT 시그널 핸들러 등록
     * - SIGINT: Ctrl+C로 발생하는 인터럽트 시그널 (2번)
     * - sigint_signalhandler: 시그널 발생 시 호출될 함수
     * -------------------------------------------------------- */
    signal (SIGINT, sigint_signalhandler);
    
    /* 무한 루프: 표준입력에서 읽어서 표준출력으로 쓰기 */
    while (1) {
        size = read(0, temp, BUFSIZ);   /* stdin(0)에서 읽기 */
        write (1, temp, size);          /* stdout(1)으로 쓰기 */
    }
    
    return 0;
}

/* ============================================================
 * sigint_signalhandler - SIGINT 시그널 핸들러 함수
 * 
 * Parameter:
 *   sig - 발생한 시그널 번호 (SIGINT는 2)
 * 
 * 동작:
 *   시그널 번호를 출력하고 프로그램 종료
 * ============================================================ */
void sigint_signalhandler (int sig) {
    printf ("SIGINT Received. [%d]\n", sig);
    exit(1);    /* 프로그램 종료 */
}
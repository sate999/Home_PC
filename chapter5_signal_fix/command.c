/* ============================================================
 * command.c - execl 후 시그널 상태 예제 (주석 있는 버전)
 * 
 * 기능: signal() 설정 후 execl() 실행 시 시그널 핸들러 동작 확인
 *       - SIG_IGN으로 설정된 시그널: execl 후에도 무시됨
 *       - 핸들러 함수로 설정된 시그널: execl 후 기본 동작으로 리셋됨
 * 컴파일: gcc -o command command.c
 * 실행: ./command /bin/cat
 * 
 * 실습 방법:
 * 1. ./command /bin/cat 실행
 * 2. ^\ (SIGQUIT) 입력 → 무시됨 (SIG_IGN 유지)
 * 3. ^C (SIGINT) 입력 → 프로그램 종료 (기본 동작으로 리셋됨)
 * ============================================================ */

#include <stdio.h>          /* printf 함수 */
#include <stdlib.h>         /* exit 함수 */
#include <signal.h>         /* signal 함수 */
#include <sys/types.h>      /* 타입 정의 */
#include <unistd.h>         /* execl 함수 */

void handler (int);

int main (int argc, char *argv[]) {
    /* 인자 검사 */
    if (argc < 2) {
        printf("Usage: %s command\n", argv[0]);
        exit(1);
    }
    
    /* --------------------------------------------------------
     * 시그널 설정
     * - SIGQUIT: SIG_IGN → execl 후에도 무시됨
     * - SIGINT: handler → execl 후 기본 동작(종료)으로 리셋됨
     * -------------------------------------------------------- */
    signal (SIGQUIT, SIG_IGN);      /* ^\를 무시 */
    signal (SIGINT, handler);       /* ^C에 핸들러 등록 */
    
    /* --------------------------------------------------------
     * execl()로 다른 프로그램 실행
     * - 현재 프로세스가 argv[1] 프로그램으로 대체됨
     * - SIG_IGN 설정은 유지됨
     * - 핸들러 설정은 기본 동작으로 리셋됨
     * -------------------------------------------------------- */
    execl (argv[1], argv[1], (char *)0);
    
    /* execl 실패 시에만 실행됨 */
    perror ("execl() :");
    exit(2);
}

void handler (int sigNo) {
    printf("Signal handler.\n");
}
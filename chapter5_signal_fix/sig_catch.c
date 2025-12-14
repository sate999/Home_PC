/* ============================================================
 * sigcatch.c - 부모-자식 시그널 핸들러 예제 (주석 있는 버전)
 * 
 * 기능: 부모와 자식 프로세스에서 각각 다른 시그널 핸들러 등록
 *       키보드로 시그널 발생 시 포그라운드 프로세스 그룹에만 전달됨
 * 컴파일: gcc -o sigcatch sigcatch.c
 * 실행: ./sigcatch
 * 
 * 테스트:
 * 1. ^\ (SIGQUIT) 입력 시 부모와 자식 모두 시그널 수신
 * 2. 포그라운드 프로세스 그룹의 모든 프로세스가 시그널 수신
 * ============================================================ */

#include <signal.h>         /* signal 함수, SIGQUIT */
#include <stdio.h>          /* printf 함수 */
#include <unistd.h>         /* fork, sleep 함수 */

/* 자식 프로세스용 시그널 핸들러 */
void child (int sig) {
    printf ("Child: signal occurred.\n");
}

/* 부모 프로세스용 시그널 핸들러 */
void parent (int sig) {
    printf ("Parent: signal occurred.\n");
}

int main () {
    /* --------------------------------------------------------
     * fork()로 자식 프로세스 생성
     * - fork() 후 부모와 자식은 각각 독립적인 시그널 핸들러 설정 가능
     * -------------------------------------------------------- */
    if (fork() == 0) {              /* 자식 프로세스 */
        signal (SIGQUIT, child);    /* 자식용 핸들러 등록 */
        while (1)
            sleep (1);
    }
    
    /* 부모 프로세스 */
    signal (SIGQUIT, parent);       /* 부모용 핸들러 등록 */
    while (1)
        sleep (1);
    
    return 0;
}
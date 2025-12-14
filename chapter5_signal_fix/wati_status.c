/* ============================================================
 * wait_status.c - wait() 상태 분석 예제 (주석 있는 버전)
 * 
 * 기능: 자식 프로세스 종료 상태를 WIFEXITED, WEXITSTATUS로 분석
 * 컴파일: gcc -o wait_status wait_status.c
 * 실행: ./wait_status
 * 
 * 출력 예시:
 * Child exit status: 1
 * 
 * wait status 분석 매크로:
 * - WIFEXITED(stat): 자식이 정상 종료했으면 참
 * - WEXITSTATUS(stat): exit() 인자값 반환
 * - WIFSIGNALED(stat): 시그널에 의해 종료했으면 참
 * - WTERMSIG(stat): 종료 시그널 번호 반환
 * ============================================================ */

#include <stdio.h>          /* printf 함수 */
#include <unistd.h>         /* fork 함수 */
#include <stdlib.h>         /* exit 함수 */
#include <sys/wait.h>       /* wait, WIFEXITED, WEXITSTATUS */
#include <sys/types.h>      /* pid_t 타입 */

int main() { 
    int stat;
    
    if (fork() == 0) {      /* 자식 프로세스 */
        exit(1);            /* exit code = 1 */
    }
    else {                  /* 부모 프로세스 */
        wait(&stat);        /* 자식 종료 대기 */
    }
    
    /* --------------------------------------------------------
     * WIFEXITED: 자식이 exit()로 정상 종료했는지 확인
     * WEXITSTATUS: exit()의 인자값 추출
     * -------------------------------------------------------- */
    if (WIFEXITED(stat))
        printf("Child exit status: %d\n", WEXITSTATUS(stat));
    
    return 0;
}
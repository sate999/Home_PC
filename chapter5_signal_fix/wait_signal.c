/* ============================================================
 * wait_signal.c - 시그널 종료 상태 분석 예제 (주석 있는 버전)
 * 
 * 기능: 자식이 알람 시그널로 종료되었을 때 상태 분석
 * 컴파일: gcc -o wait_signal wait_signal.c
 * 실행: ./wait_signal
 * 
 * 출력 예시:
 * Child exit signal # = [14]
 * Child exit signal: Alarm clock
 * 
 * wait status 분석 매크로:
 * - WIFSIGNALED(stat): 시그널에 의해 종료했으면 참
 * - WTERMSIG(stat): 종료 시그널 번호 반환
 * - psignal(): 시그널 번호에 해당하는 설명 출력
 * ============================================================ */

#include <stdio.h>          /* printf 함수 */
#include <unistd.h>         /* fork, alarm 함수 */
#include <stdlib.h>         /* exit 함수 */
#include <sys/wait.h>       /* wait, WIFSIGNALED, WTERMSIG */
#include <sys/types.h>      /* pid_t 타입 */

int main() { 
    int stat; 
    pid_t pid;
    
    if ((pid = fork()) == 0) {      /* 자식 프로세스 */
        /* --------------------------------------------------------
         * alarm(1): 1초 후 SIGALRM 시그널 발생
         * while(1): 무한 루프 (알람 시그널에 의해 종료됨)
         * - 핸들러 없으므로 기본 동작(종료) 수행
         * -------------------------------------------------------- */
        alarm (1);
        while (1);
    }
    else {                          /* 부모 프로세스 */
        wait(&stat);                /* 자식 종료 대기 */
    }
    
    /* --------------------------------------------------------
     * WIFSIGNALED: 시그널에 의해 종료했는지 확인
     * WTERMSIG: 종료시킨 시그널 번호 추출
     * psignal: 시그널 설명 출력
     * -------------------------------------------------------- */
    if (WIFSIGNALED(stat)) {
        printf ("Child exit signal # = [%d]\n", WTERMSIG(stat));
        psignal(WTERMSIG(stat), "Child exit signal");
    }
    
    return 0;
}
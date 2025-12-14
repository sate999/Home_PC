/* ============================================================
 * sig_ignore.c - 시그널 무시 예제 (주석 있는 버전)
 * 
 * 기능: SIG_IGN을 사용하여 SIGINT와 SIGQUIT 시그널 무시
 *       이 프로그램은 Ctrl+C나 Ctrl+\로 종료되지 않음
 * 컴파일: gcc -o sig_ignore sig_ignore.c
 * 실행: ./sig_ignore
 * 종료: kill -9 <PID> 또는 다른 터미널에서 kill 명령 사용
 * 
 * 주의:
 * - SIGKILL(9)과 SIGSTOP은 무시하거나 변경할 수 없음
 * ============================================================ */

#include <signal.h>         /* signal 함수, SIG_IGN, SIGINT, SIGQUIT */
#include <unistd.h>         /* sleep 함수 */

int main() {
    /* --------------------------------------------------------
     * SIG_IGN: 시그널 무시 (Ignore)
     * - SIGINT(^C)와 SIGQUIT(^\)를 무시하도록 설정
     * - 이 프로그램은 Ctrl+C나 Ctrl+\로 종료되지 않음
     * -------------------------------------------------------- */
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    
    /* 무한 루프: 프로그램 계속 실행 */
    while (1)
        sleep (1);  /* 1초마다 대기 (CPU 사용량 감소) */
    
    return 0;
}
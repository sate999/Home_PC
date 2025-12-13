/* ============================================
 * Detach Example: pthread_detach + exit(0)
 * - pthread_detach(): 스레드를 분리 상태로 설정
 * - 분리된 스레드는 종료 시 자동으로 자원 회수
 * - exit(0) 사용 시 main이 종료되면 모든 스레드 종료
 * 컴파일: gcc -o detach detach.c -pthread
 * ============================================ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // sleep() 함수 사용
#include <pthread.h>

/* 스레드 함수
 * 무한 루프로 3초마다 메시지 출력 */
void *func(void *data) {
    while (1) {
        printf("Speaking from the detached thread...\n");
        sleep(3);   // 3초 대기
    }
}

int main() {
    pthread_t handle;   // 스레드 ID
    
    /* 스레드 생성
     * pthread_create 성공 시 0 반환
     * !0 = 1 (true) 이므로 성공 시 메시지 출력 */
    if (!pthread_create(&handle, NULL, func, NULL))
        printf("Thread created successfully !!!\n");
    
    /* pthread_detach(): 스레드를 분리 상태로 설정
     * 분리된 스레드가 종료하면 자원이 자동으로 시스템에 반환됨
     * pthread_join()을 호출할 필요 없음 */
    if (!pthread_detach(handle))
        printf("Thread detached successfully !!!\n");
    
    sleep(3);   // 3초 대기 (스레드가 한 번 실행될 시간 제공)
    
    printf("Main thread dying...\n");
    
    /* exit(0): 프로세스 종료
     * main이 종료되면 분리된 스레드도 함께 종료됨!
     * 스레드가 detach 되어도 프로세스 종료 시에는 같이 종료 */
    exit(0);
    // pthread_exit((void *)0);  // 이렇게 하면 스레드가 계속 실행됨
}

/* 실행 결과:
 * Thread created successfully !!!
 * Thread detached successfully !!!
 * Speaking from the detached thread...
 * Main thread dying...
 * (프로그램 종료)
 */
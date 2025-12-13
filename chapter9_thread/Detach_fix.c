/* ============================================
 * Detach Example 수정: pthread_exit() 사용
 * - exit(0) 대신 pthread_exit() 사용
 * - main 스레드가 pthread_exit()하면 다른 스레드 계속 실행
 * - 모든 스레드가 종료되어야 프로세스 종료
 * 컴파일: gcc -o detach2 detach2.c -pthread
 * ============================================ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* 스레드 함수
 * 무한 루프로 3초마다 메시지 출력 */
void *func(void *data) {
    while (1) {
        printf("Speaking from the detached thread...\n");
        sleep(3);
    }
}

int main() {
    pthread_t handle;
    
    if (!pthread_create(&handle, NULL, func, NULL))
        printf("Thread created successfully !!!\n");
    
    if (!pthread_detach(handle))
        printf("Thread detached successfully !!!\n");
    
    sleep(3);
    
    printf("Main thread dying...\n");
    
    /* pthread_exit(): main 스레드만 종료
     * 다른 스레드들은 계속 실행됨!
     * exit(0)과의 차이점: exit(0)은 프로세스 전체 종료 */
    // exit(0);
    pthread_exit((void *)0);
}

/* 실행 결과:
 * Thread created successfully !!!
 * Thread detached successfully !!!
 * Speaking from the detached thread...
 * Main thread dying...
 * Speaking from the detached thread...  (계속 반복)
 * Speaking from the detached thread...
 * ... (Ctrl+C로 종료해야 함)
 */
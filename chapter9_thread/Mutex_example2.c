/* ============================================
 * Mutex Example 2 (Race Condition): 또 다른 race condition 예제
 * - Example 1과 유사하지만 다른 구조
 * - counter++ 후 시간 지연 → counter 값 출력
 * - 두 스레드가 counter를 동시에 수정하면 둘 다 2를 출력할 수 있음
 * 컴파일: gcc -o mutex2_no mutex2_no.c -pthread
 * ============================================ */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *function();       // 스레드 함수 선언
int counter = 0;        // 전역 공유 변수

int main() {
    int rc1, rc2;
    pthread_t thread1, thread2;
    
    /* 첫 번째 스레드 생성 */
    if ((rc1 = pthread_create(&thread1, NULL, function, NULL))) {
        printf("Thread creation failed: %d\n", rc1);
    }
    
    /* 두 번째 스레드 생성 */
    if ((rc2 = pthread_create(&thread2, NULL, function, NULL))) {
        printf("Thread creation failed: %d\n", rc2);
    }
    
    /* 두 스레드 종료 대기 */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    exit(EXIT_SUCCESS);
}

void *function() {
    int i;
    
    /* Race condition 발생 지점!
     * 스레드 1이 counter++한 직후 스레드 2도 counter++ 할 수 있음 */
    counter++;
    
    /* 시간 지연: 이 사이에 다른 스레드가 counter를 변경할 수 있음 */
    for (i = 0; i < 0xFFFFFFF; i++)
        ;
    
    /* 출력 시점에 counter가 이미 변경되어 있을 수 있음 */
    printf("Counter value: %d\n", counter);
    
    return NULL;
}

/* 실행 결과 (race condition으로 예측 불가):
 * Counter value: 2
 * Counter value: 2
 * 
 * 또는:
 * Counter value: 1
 * Counter value: 2
 */
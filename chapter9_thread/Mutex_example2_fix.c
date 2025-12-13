/* ============================================
 * Mutex Example 2 수정: 정적 뮤텍스 초기화 사용
 * - PTHREAD_MUTEX_INITIALIZER로 정적 초기화
 * - pthread_mutex_init() 호출 불필요
 * - 임계영역 보호로 race condition 해결
 * 컴파일: gcc -o mutex2 mutex2.c -pthread
 * ============================================ */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *function();
int counter = 0;

/* 뮤텍스 정적 초기화
 * pthread_mutex_init()을 호출하지 않아도 됨
 * 전역 변수로 선언 시 PTHREAD_MUTEX_INITIALIZER 사용 가능 */
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main() {
    int rc1, rc2;
    pthread_t thread1, thread2;
    
    if ((rc1 = pthread_create(&thread1, NULL, function, NULL))) {
        printf("Thread creation failed: %d\n", rc1);
    }
    
    if ((rc2 = pthread_create(&thread2, NULL, function, NULL))) {
        printf("Thread creation failed: %d\n", rc2);
    }
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    exit(EXIT_SUCCESS);
}

void *function() {
    int i;
    
    /* 뮤텍스 잠금: 임계영역 보호 시작 */
    pthread_mutex_lock(&mutex1);
    
    /* ===== 임계영역 시작 ===== */
    counter++;
    
    for (i = 0; i < 0xFFFFFFF; i++)
        ;
    
    printf("Counter value: %d\n", counter);
    /* ===== 임계영역 끝 ===== */
    
    /* 뮤텍스 해제 */
    pthread_mutex_unlock(&mutex1);
    
    return NULL;
}

/* 실행 결과 (항상 순서대로):
 * Counter value: 1
 * Counter value: 2
 */
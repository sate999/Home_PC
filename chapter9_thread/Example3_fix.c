/* ============================================
 * Example 3 수정: pthread_join이 없는 경우
 * - main 스레드가 종료되면 모든 스레드가 함께 종료됨
 * - 스레드들이 실행을 완료하기 전에 프로그램이 끝날 수 있음
 * 컴파일: gcc -o ex3_nojoin ex3_nojoin.c -pthread
 * ============================================ */

#include <stdio.h>
#include <pthread.h>

#define NTHREADS 5

int counter = 0;

void *thread_function(void *dummyPtr) {
    printf("Thread number %ld\n", pthread_self());
    counter++;
    return NULL;
}

void main() {
    pthread_t thread_id[NTHREADS];
    int i, j;
    
    /* 스레드 생성 */
    for (i = 0; i < NTHREADS; i++) {
        pthread_create(&thread_id[i], NULL, thread_function, NULL);
    }
    
    /* pthread_join 없음!
     * main 스레드가 바로 printf 실행 후 종료됨
     * main이 return하면 모든 스레드가 강제 종료됨 */
    /*
    for (j = 0; j < NTHREADS; j++) {
        pthread_join(thread_id[j], NULL);
    }
    */
    
    /* 스레드들이 counter를 증가시키기 전에 출력될 수 있음
     * 결과가 0, 1, 2, ... 등 예측 불가 */
    printf("Final counter value: %d\n", counter);
}

/* 실행 결과 (예측 불가):
 * Final counter value: 0
 * (또는 1, 2, ... 스레드 실행 상황에 따라 다름)
 * 
 * 주의: main이 return하거나 exit(3)을 호출하면
 *       모든 스레드가 즉시 종료됨!
 */
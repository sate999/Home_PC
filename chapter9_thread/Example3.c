/* ============================================
 * Example 3: 다중 스레드 생성 및 전역 변수 공유
 * - 여러 개의 스레드를 배열로 관리
 * - 전역 변수 counter를 모든 스레드가 공유
 * - pthread_self()로 자신의 스레드 ID 확인
 * 컴파일: gcc -o ex3 ex3.c -pthread
 * ============================================ */

#include <stdio.h>
#include <pthread.h>

#define NTHREADS 5      // 생성할 스레드 개수

int counter = 0;        // 전역 변수: 모든 스레드가 공유

/* 스레드 함수
 * 각 스레드는 자신의 ID를 출력하고 counter를 증가시킴 */
void *thread_function(void *dummyPtr) {
    /* pthread_self(): 현재 실행 중인 스레드의 ID 반환
     * %ld 형식으로 출력 (pthread_t는 보통 unsigned long 타입) */
    printf("Thread number %ld\n", pthread_self());
    
    /* 주의: 이 부분에서 스레드 스위칭이 발생할 수 있음
     * 여러 스레드가 counter에 동시에 접근하면 race condition 발생 가능 */
    counter++;
    
    return NULL;    // 반환값 없음
}

void main() {
    pthread_t thread_id[NTHREADS];  // 스레드 ID를 저장할 배열
    int i, j;
    
    /* NTHREADS(5)개의 스레드 생성 */
    for (i = 0; i < NTHREADS; i++) {
        pthread_create(&thread_id[i], NULL, thread_function, NULL);
    }
    
    /* 모든 스레드가 종료될 때까지 대기
     * pthread_join의 두 번째 인자가 NULL이면 반환값을 받지 않음 */
    for (j = 0; j < NTHREADS; j++) {
        pthread_join(thread_id[j], NULL);
    }
    
    /* 모든 스레드가 종료된 후 최종 counter 값 출력
     * 정상적으로 실행되면 5가 출력됨 */
    printf("Final counter value: %d\n", counter);
}

/* 실행 결과 (스레드 ID는 실행마다 다름):
 * Thread number 140234567890432
 * Thread number 140234567890433
 * Thread number 140234567890434
 * Thread number 140234567890435
 * Thread number 140234567890436
 * Final counter value: 5
 */
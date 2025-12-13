/* ============================================
 * Mutex Example 3: pthread_mutex_trylock() 사용
 * - pthread_mutex_trylock(): 비블로킹 잠금 시도
 * - 뮤텍스가 이미 잠겨있으면 EBUSY 반환하고 즉시 리턴
 * - pthread_mutex_lock()과 달리 대기하지 않음
 * 컴파일: gcc -o mutex3 mutex3.c -pthread
 * ============================================ */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>      // EBUSY 상수 정의

pthread_mutex_t mutex;  // 뮤텍스 변수

/* 스레드 함수
 * trylock으로 뮤텍스 획득 시도 */
void *thread(void *arg) {
    int ret;
    
    /* pthread_mutex_trylock(): 뮤텍스 잠금 시도
     * - 뮤텍스가 unlocked 상태면: lock 획득, 0 반환
     * - 뮤텍스가 locked 상태면: block하지 않고 즉시 EBUSY 반환 */
    if ((ret = pthread_mutex_trylock(&mutex)) != 0) {
        if (ret == EBUSY)
            /* 다른 스레드가 이미 뮤텍스를 가지고 있음 */
            puts("thread was denied access to the mutex");
        else {
            perror("pthread_mutex_trylock() error");
            exit(1);
        }
    }
    else {
        /* 뮤텍스 획득 성공 */
        puts("thread was granted the mutex");
        /* 여기서 임계영역 작업 수행 후 unlock 해야 함 */
        // pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

void main() {
    int ret;
    pthread_t thid;
    
    /* 뮤텍스 동적 초기화 */
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex_init() error");
        exit(2);
    }
    
    /* 스레드 생성 */
    if (pthread_create(&thid, NULL, thread, NULL) != 0) {
        perror("pthread_create() error");
        exit(3);
    }
    
    /* main 스레드에서도 뮤텍스 획득 시도
     * 스레드와 main이 거의 동시에 trylock을 시도
     * 먼저 도착한 쪽이 뮤텍스 획득 */
    if ((ret = pthread_mutex_trylock(&mutex)) != 0) {
        if (ret == EBUSY)
            puts("main was denied access to the mutex");
        else {
            perror("pthread_mutex_trylock() error");
            exit(4);
        }
    }
    else {
        puts("main was granted the mutex");
    }
    
    /* 스레드 종료 대기 */
    if (pthread_join(thid, NULL) != 0) {
        perror("pthread_join() error");
        exit(5);
    }
}

/* 실행 결과 (실행마다 다를 수 있음):
 * main was granted the mutex
 * thread was denied access to the mutex
 * 
 * 또는:
 * thread was granted the mutex
 * main was denied access to the mutex
 */
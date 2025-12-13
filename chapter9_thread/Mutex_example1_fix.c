/* ============================================
 * Mutex Example 1 수정: 뮤텍스를 사용하여 race condition 해결
 * - pthread_mutex_init(): 뮤텍스 초기화
 * - pthread_mutex_lock(): 임계영역 진입 전 잠금
 * - pthread_mutex_unlock(): 임계영역 종료 후 해제
 * - 한 번에 하나의 스레드만 임계영역 실행
 * 컴파일: gcc -o mutex1 mutex1.c -pthread
 * ============================================ */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];           // 2개의 스레드 ID 배열
int counter = 0;            // 공유 변수
pthread_mutex_t lock;       // 뮤텍스 변수 선언

void *doSomeThing(void *arg);

int main(void) {
    int i = 0, err;
    
    /* pthread_mutex_init(): 뮤텍스 초기화
     * 첫 번째 인자: 뮤텍스 변수의 주소
     * 두 번째 인자: 속성 (NULL이면 기본 속성)
     * 반환값: 성공 시 0 */
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }
    
    /* 2개의 스레드 생성 */
    while (i < 2) {
        if (pthread_create(&(tid[i]), NULL, doSomeThing, NULL))
            printf("can't create thread\n");
        i++;
    }
    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    
    /* pthread_mutex_destroy(): 뮤텍스 해제 (선택사항) */
    // pthread_mutex_destroy(&lock);
    
    return 0;
}

void *doSomeThing(void *arg) {
    unsigned long i = 0;
    
    /* pthread_mutex_lock(): 뮤텍스 잠금
     * 이미 다른 스레드가 lock을 획득했다면 block(대기)
     * lock을 획득할 때까지 여기서 멈춤 */
    pthread_mutex_lock(&lock);
    
    /* ===== 임계영역(Critical Section) 시작 ===== */
    counter += 1;
    
    printf("Job %d started\n", counter);
    
    for (i = 0; i < (0xFFFFFFF); i++)
        ;
    
    printf("Job %d finished\n", counter);
    /* ===== 임계영역(Critical Section) 끝 ===== */
    
    /* pthread_mutex_unlock(): 뮤텍스 해제
     * 대기 중인 다른 스레드가 lock을 획득할 수 있게 됨 */
    pthread_mutex_unlock(&lock);
    
    return NULL;
}

/* 실행 결과 (항상 순서대로):
 * Job 1 started
 * Job 1 finished
 * Job 2 started
 * Job 2 finished
 */
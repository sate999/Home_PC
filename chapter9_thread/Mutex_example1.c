/* ============================================
 * Mutex Example 1 (Race Condition): 뮤텍스 없이 공유 변수 접근
 * - 두 스레드가 counter 변수에 동시 접근
 * - Race condition으로 인해 예상치 못한 결과 발생
 * - Job 1 started → Job 2 started → Job 2 finished → Job 2 finished
 *   처럼 순서가 뒤바뀜
 * 컴파일: gcc -o mutex1_no mutex1_no.c -pthread
 * ============================================ */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];       // 2개의 스레드 ID 배열
int counter = 0;        // 공유 변수 (race condition 발생 지점)

/* 스레드 함수 선언 */
void *doSomeThing(void *arg);

int main(void) {
    int i = 0;
    
    /* 2개의 스레드 생성 */
    while (i < 2) {
        if (pthread_create(&(tid[i]), NULL, doSomeThing, NULL))
            printf("can't create thread\n");
        i++;
    }
    
    /* 두 스레드가 종료될 때까지 대기 */
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    
    return 0;
}

/* 스레드 함수
 * counter 증가 후 작업 시뮬레이션 */
void *doSomeThing(void *arg) {
    unsigned long i = 0;
    
    /* 문제 발생 지점!
     * 두 스레드가 거의 동시에 counter를 읽고 수정할 수 있음
     * 스레드 1이 counter를 1로 만든 직후
     * 스레드 2가 counter를 2로 만들 수 있음 */
    counter += 1;
    
    printf("Job %d started\n", counter);
    
    /* 시간이 오래 걸리는 작업 시뮬레이션 */
    for (i = 0; i < (0xFFFFFFF); i++)
        ;   // 빈 루프로 시간 지연
    
    /* 이 시점에서 counter 값이 이미 바뀌어 있을 수 있음! */
    printf("Job %d finished.\n", counter);
    
    return NULL;
}

/* 실행 결과 (예측 불가, race condition):
 * Job 1 started
 * Job 2 started      <- counter가 이미 2가 됨
 * Job 2 finished     <- 둘 다 2를 출력
 * Job 2 finished
 * 
 * 또는:
 * Job 2 started
 * Job 2 started
 * Job 2 finished
 * Job 2 finished
 */
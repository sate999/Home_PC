/* ============================================
 * 연습문제 1: argc 값을 스레드에 전달하고 +3 반환
 * - main의 argc를 스레드에 전달
 * - 스레드는 argc + 3 값을 반환
 * - main은 반환값을 %d로 출력
 * 컴파일: gcc -o practice1 practice1.c -pthread
 * 실행: ./practice1 (argc=1), ./practice1 a b (argc=3)
 * ============================================ */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* 스레드 함수
 * 매개변수로 전달받은 argc 값에 3을 더해서 반환 */
void *thread_func(void *arg) {
    /* arg는 void* 타입이므로 int*로 캐스팅 후 역참조하여 값을 얻음
     * 또는 직접 (long)arg 또는 (int)(long)arg로 캐스팅 가능 */
    int argc_value = *(int *)arg;   // 전달받은 argc 값
    int *result;                     // 결과를 저장할 동적 메모리 포인터
    
    /* 결과값을 저장할 메모리 할당 */
    if ((result = malloc(sizeof(int))) == NULL) {
        perror("malloc error");
        exit(1);
    }
    
    *result = argc_value + 3;   // argc + 3 계산
    
    return result;              // 결과 메모리 주소 반환
}

int main(int argc, char *argv[]) {
    pthread_t tid;      // 스레드 ID
    void *ret;          // 스레드 반환값을 저장할 포인터
    
    /* 스레드 생성
     * &argc: argc 변수의 주소를 스레드에 전달 */
    if (pthread_create(&tid, NULL, thread_func, &argc) != 0) {
        printf("pthread_create error\n");
        exit(1);
    }
    
    /* 스레드 종료 대기
     * ret에 스레드가 반환한 malloc된 메모리 주소가 저장됨 */
    if (pthread_join(tid, &ret) != 0) {
        printf("pthread_join error\n");
        exit(1);
    }
    
    /* 반환값 출력: %d 형식으로 출력
     * ret을 int*로 캐스팅 후 역참조 */
    printf("%d\n", *(int *)ret);
    
    free(ret);  // 동적 할당 메모리 해제
    
    return 0;
}

/* 실행 결과:
 * $ ./practice1         -> 4  (argc=1, 1+3=4)
 * $ ./practice1 a       -> 5  (argc=2, 2+3=5)
 * $ ./practice1 a b c   -> 7  (argc=4, 4+3=7)
 */
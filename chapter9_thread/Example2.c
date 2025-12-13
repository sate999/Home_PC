/* ============================================
 * Example 2: pthread_exit()을 이용한 스레드 종료
 * - malloc()으로 동적 메모리 할당
 * - pthread_exit()으로 스레드 종료 및 값 반환
 * - pthread_join()으로 반환된 포인터 수신
 * 컴파일: gcc -o ex2 ex2.c -pthread
 * ============================================ */

#include <stdlib.h>     // malloc(), free(), exit() 사용
#include <stdio.h>      // printf(), perror() 사용
#include <string.h>     // strcpy() 사용
#include <pthread.h>    // pthread 관련 함수들 사용

/* 스레드 함수
 * 동적으로 메모리를 할당하고 문자열을 복사한 후
 * pthread_exit()으로 해당 메모리의 주소를 반환 */
void *thread(void *arg) {
    char *ret;  // 동적 할당된 메모리를 가리킬 포인터
    
    // 전달받은 인자(문자열)를 출력
    printf("thread() entered with argument '%s'\n", (char *)arg);
    
    /* 20바이트 메모리 동적 할당
     * 실패 시 에러 메시지 출력 후 종료 */
    if ((ret = (char *)malloc(20)) == NULL) {
        perror("malloc() error");
        exit(2);
    }
    
    strcpy(ret, "This is a test");  // 할당된 메모리에 문자열 복사
    
    /* pthread_exit(): 스레드 종료
     * ret (문자열이 저장된 메모리 주소)를 반환값으로 전달
     * 이 값은 pthread_join()의 두 번째 파라미터로 전달됨 */
    pthread_exit(ret);
}

void main() {
    pthread_t thid;     // 스레드 ID
    void *rt;           // 스레드의 반환값(포인터)을 저장할 변수
    
    /* 스레드 생성: "thread 1" 문자열을 인자로 전달 */
    if (pthread_create(&thid, NULL, thread, "thread 1") != 0) {
        perror("pthread_create() error");
        exit(1);
    }
    
    /* pthread_join(): 스레드 종료 대기
     * &rt: 스레드가 pthread_exit()으로 전달한 값(포인터)이 rt에 저장됨
     * 즉, rt에는 "This is a test" 문자열이 저장된 메모리 주소가 들어감 */
    if (pthread_join(thid, &rt) != 0) {
        perror("pthread_join() error");
        exit(3);
    }
    
    // rt를 char*로 캐스팅하여 문자열 출력
    printf("thread exited with '%s'\n", (char *)rt);
    
    // 동적 할당된 메모리 해제 (메모리 누수 방지)
    free(rt);
}

/* 실행 결과:
 * thread() entered with argument 'thread 1'
 * thread exited with 'This is a test'
 */
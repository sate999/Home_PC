/* ============================================
 * Example 1 (다른 방법): 정수값을 동적 메모리로 반환
 * - 13페이지 Example1과 동일한 기능
 * - 차이점: 정수를 직접 반환하지 않고 malloc으로 할당한 메모리에 저장
 * - 지역변수의 주소를 반환하면 안 되는 이유 설명
 * 컴파일: gcc -o ex1_alt ex1_alt.c -pthread
 * ============================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

/* 스레드 함수
 * 문자열 길이를 동적 메모리에 저장하여 반환 */
void *threadFunc(void *arg) {
    char *s = (char *)arg;  // 전달받은 문자열
    int *ret;               // 동적 할당할 int형 포인터
    int len;                // 문자열 길이 저장용 지역변수
    
    printf("\tT: %s", s);   // 문자열 출력
    
    len = strlen(s);        // 문자열 길이 계산 (12)
    
    /* 주의: return (&len); 은 잘못된 방법!
     * len은 지역변수이므로 함수 종료 시 메모리가 해제됨
     * 해제된 메모리 주소를 반환하면 undefined behavior 발생 */
    // return (&len);   // 절대 하면 안 됨!
    
    /* 올바른 방법: malloc으로 힙 메모리 할당 */
    if ((ret = malloc(sizeof(int))) == NULL) {
        perror("malloc() error");
        exit(2);
    }
    
    *ret = len;     // 할당된 메모리에 길이값 저장
    return ret;     // 힙 메모리 주소 반환 (함수 종료 후에도 유효)
}

int main() {
    pthread_t t1;
    void *res;      // 스레드 반환값(포인터)을 저장
    
    if (pthread_create(&t1, NULL, threadFunc, "Hello world\n") != 0)
        printf("pthread_create error\n");
    
    printf("Message from main()\n");
    
    /* pthread_join으로 스레드의 반환값 수신
     * res에는 malloc으로 할당된 메모리 주소가 저장됨 */
    if (pthread_join(t1, &res) != 0)
        printf("pthread_join error\n");
    
    /* res를 int*로 캐스팅하고 역참조하여 값 출력 */
    printf("Thread returned %d\n", *(int *)res);
    
    free(res);      // 동적 할당 메모리 해제
    exit(EXIT_SUCCESS);
}

/* 실행 결과:
 * Message from main()
 *     T: Hello world
 * Thread returned 12
 */
/* ============================================
 * Example 1: 기본 스레드 생성과 pthread_join
 * - pthread_create(): 스레드 생성
 * - pthread_join(): 스레드 종료 대기 및 반환값 수신
 * 컴파일: gcc -o ex1 ex1.c -pthread
 * ============================================ */

#include <stdio.h>      // printf() 함수 사용
#include <string.h>     // strlen() 함수 사용
#include <stdlib.h>     // exit(), EXIT_SUCCESS 사용
#include <pthread.h>    // pthread 관련 함수들 사용

/* 스레드가 실행할 함수
 * 매개변수: void *arg - pthread_create에서 전달한 인자 (여기서는 문자열)
 * 반환값: void * - 문자열 길이를 void* 타입으로 캐스팅하여 반환 */
void *threadFunc(void *arg) {
    char *s = (char *)arg;      // void* 타입을 char* 타입으로 캐스팅
    printf("\tT: %s", s);       // 전달받은 문자열 출력 (\t는 탭)
    // getchar();               // 주석 해제 시 키 입력 대기
    return ((void *)strlen(s)); // 문자열 길이(12)를 void*로 캐스팅하여 반환
}

int main() {
    pthread_t t1;   // 스레드 ID를 저장할 변수
    int res;        // 스레드의 반환값을 저장할 변수
    
    /* pthread_create(): 새로운 스레드 생성
     * 파라미터:
     *   &t1 - 생성된 스레드의 ID가 저장될 변수의 주소
     *   NULL - 스레드 속성 (NULL이면 기본 속성 사용)
     *   threadFunc - 스레드가 실행할 함수
     *   "Hello world\n" - threadFunc에 전달할 인자
     * 반환값: 성공 시 0, 실패 시 에러 번호 */
    if (pthread_create(&t1, NULL, threadFunc, "Hello world\n") != 0)
        printf("pthread_create error\n");
    
    printf("Message from main()\n");    // main 스레드에서 출력
    
    /* pthread_join(): 스레드 t1이 종료될 때까지 대기
     * 파라미터:
     *   t1 - 대기할 스레드의 ID
     *   (void**)&res - 스레드의 반환값을 저장할 포인터의 주소
     * 스레드가 종료되면 반환값(strlen 결과)이 res에 저장됨 */
    if (pthread_join(t1, (void**)&res) != 0)
        printf("pthread_join error\n");
    
    printf("Thread returned %d\n", res);    // 스레드 반환값 출력 (12)
    exit(EXIT_SUCCESS);                      // 프로그램 정상 종료
}

/* 실행 결과:
 * Message from main()
 *     T: Hello world
 * Thread returned 12
 */
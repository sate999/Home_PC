/* ============================================================
 * setjmp_example.c - setjmp/longjmp 예제 (주석 있는 버전)
 * 
 * 기능: non-local goto (함수 간 점프) 구현
 * 컴파일: gcc -o setjmp_example setjmp_example.c
 * 실행: ./setjmp_example (Ctrl+C로 종료)
 * 
 * 출력 예시:
 * [0]
 * 3
 * -->[2]
 * 3
 * -->[2]
 * 3
 * ... (무한 반복)
 * 
 * setjmp/longjmp 동작:
 * - setjmp(env): 현재 상태를 env에 저장, 처음 호출 시 0 반환
 * - longjmp(env, val): setjmp 위치로 점프, val 값 반환
 * ============================================================ */

#include <stdio.h>          /* printf 함수 */
#include <setjmp.h>         /* setjmp, longjmp, jmp_buf */
#include <unistd.h>         /* sleep 함수 */

jmp_buf env1;   /* 점프 위치 저장 버퍼 */

/* 이 함수에서 longjmp 호출 → setjmp 위치로 점프 */
void func () {
    longjmp (env1, 2);  /* setjmp로 점프, 반환값 2 */
}

int main () {
    int retcode;
    
    /* --------------------------------------------------------
     * setjmp(): 점프 위치 설정
     * - 처음 호출: 0 반환
     * - longjmp에 의해 반환: longjmp의 val 인자 반환 (2)
     * -------------------------------------------------------- */
    retcode = setjmp(env1);
    
    if (retcode == 0)
        printf ("[%d]\n", retcode);     /* 처음 호출 */
    else
        printf ("-->[%d]\n", retcode);  /* longjmp에 의한 호출 */
    
    printf ("3\n\n");
    sleep (1);
    
    func();     /* longjmp 호출 → setjmp로 점프 */
    
    return 0;
}
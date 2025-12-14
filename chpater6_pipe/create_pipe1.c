/* ============================================================
 * create_pipe1.c - 양방향 파이프 통신 예제 (주석 있는 버전)
 * 
 * 기능: 두 개의 파이프를 사용하여 부모-자식 간 양방향 통신
 *       사용자 입력 → 부모 → (send_fd) → 자식 → (rcv_fd) → 부모
 * 컴파일: gcc -o create_pipe1 create_pipe1.c
 * 실행: ./create_pipe1
 * 
 * 출력 예시:
 * this is test...  (사용자 입력)
 * [Parent] : Send Message : this is test...
 * [Child] : Receive Message : this is test...
 * [Child] : Send Message : this is test...
 * [Parent] : Receive Message : this is test...
 * ============================================================ */

#include <stdio.h>      /* printf 함수 사용 */
#include <stdlib.h>     /* exit 함수 사용 */
#include <unistd.h>     /* pipe, fork, read, write, close 함수 사용 */
#include <string.h>     /* memset 함수 사용 */

#define MAX_SIZE 50     /* 버퍼 최대 크기 */

int main() {
    int send_fd[2], rcv_fd[2], pid=0, size=0;
    /* send_fd: 부모→자식 방향 파이프 */
    /* rcv_fd: 자식→부모 방향 파이프 */
    
    char send_buf[MAX_SIZE], rcv_buf[MAX_SIZE];
    
    /* --------------------------------------------------------
     * memset으로 버퍼 초기화
     * - 0x00으로 채워서 쓰레기 값 방지
     * -------------------------------------------------------- */
    memset (send_buf, 0x00, MAX_SIZE);
    memset (rcv_buf, 0x00, MAX_SIZE);
    
    /* --------------------------------------------------------
     * 첫 번째 파이프 생성 (부모→자식 방향)
     * send_fd[0]: 읽기용 (자식이 사용)
     * send_fd[1]: 쓰기용 (부모가 사용)
     * -------------------------------------------------------- */
    if ( pipe(send_fd) == -1 ) {
        perror("pipe() : ");    /* 에러 메시지 출력 */
        exit(1);
    }
    
    /* --------------------------------------------------------
     * 두 번째 파이프 생성 (자식→부모 방향)
     * rcv_fd[0]: 읽기용 (부모가 사용)
     * rcv_fd[1]: 쓰기용 (자식이 사용)
     * -------------------------------------------------------- */
    if ( pipe(rcv_fd) == -1 ) {
        perror("pipe() : ");
        exit(2);
    }
    
    /* --------------------------------------------------------
     * fork()로 자식 프로세스 생성
     * 자식: pid == 0
     * 부모: pid > 0 (자식의 PID)
     * -------------------------------------------------------- */
    if ((pid = fork()) == 0 ) {     /* 자식 프로세스 */
        /* 사용하지 않는 파이프 끝 닫기 */
        close(send_fd[1]);          /* 부모→자식 파이프의 쓰기 끝 닫기 */
        close(rcv_fd[0]);           /* 자식→부모 파이프의 읽기 끝 닫기 */
        
        /* send_fd 파이프에서 부모가 보낸 메시지 읽기 */
        size = read(send_fd[0], rcv_buf, MAX_SIZE);
        printf("\t[Child] : Receive Message : %s\n", rcv_buf);
        
        /* rcv_fd 파이프로 부모에게 메시지 전송 (에코) */
        write(rcv_fd[1], rcv_buf, size);
        printf("\t[Child] : Send Message : %s\n", rcv_buf);
        
        exit(0);    /* 자식 프로세스 종료 */
    }
    else if ( pid > 0 ) {           /* 부모 프로세스 */
        /* 사용하지 않는 파이프 끝 닫기 */
        close(send_fd[0]);          /* 부모→자식 파이프의 읽기 끝 닫기 */
        close(rcv_fd[1]);           /* 자식→부모 파이프의 쓰기 끝 닫기 */
        
        /* 표준입력(키보드)에서 메시지 읽기 */
        size = read(0, send_buf, MAX_SIZE);
        
        /* send_fd 파이프로 자식에게 메시지 전송 */
        write(send_fd[1], send_buf, size);
        printf("[Parent] : Send Message : %s\n", send_buf);
        
        /* rcv_fd 파이프에서 자식이 보낸 메시지 읽기 (blocking) */
        /* 자식이 아직 write하지 않았으면 여기서 대기 */
        read(rcv_fd[0], rcv_buf, MAX_SIZE);
        printf("[Parent] : Receive Message : %s\n", rcv_buf);
    }
    
    return 0;
}
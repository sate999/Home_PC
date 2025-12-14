/* ============================================================
 * namedPipe.h - Named Pipe 클라이언트-서버 공통 헤더 (주석 있는 버전)
 * 
 * 용도: server.c와 client.c에서 공통으로 사용하는 구조체 및 상수 정의
 * 
 * Named Pipe (FIFO)란?
 * - 부모-자식 관계가 아닌 프로세스들 간의 통신을 위한 파이프
 * - 파일 시스템에 특수 파일로 존재
 * - mknod 명령/함수로 생성
 * ============================================================ */

#include <fcntl.h>          /* open 함수, O_RDONLY, O_WRONLY 등 */
#include <sys/types.h>      /* 타입 정의 */
#include <sys/stat.h>       /* S_IFIFO 등 파일 모드 상수 */
#include <stdlib.h>         /* exit 함수 */
#include <stdio.h>          /* printf, perror 함수 */
#include <string.h>         /* strcpy, sprintf 함수 */
#include <unistd.h>         /* read, write, close, getpid 함수 */

/* ============================================================
 * message 구조체
 * - 클라이언트가 서버에게 보내는 요청 메시지
 * - privfifo: 클라이언트가 생성한 개인 FIFO 이름 (응답용)
 * - filename: 클라이언트가 읽기를 원하는 파일 이름
 * ============================================================ */
struct message {
    char privfifo[15];      /* 개인 named pipe 이름 (예: "Fifo1234") */
    char filename[100];     /* 읽을 파일 이름 */
};

#define PUBLIC "Public"     /* 공개 named pipe 이름 (서버가 생성) */
#define LINESIZE 512        /* 한 줄 버퍼 크기 */
#define NUMTRIES 3          /* 파이프 open 재시도 횟수 */
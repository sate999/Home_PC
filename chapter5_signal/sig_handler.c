/*

목적 : SIGINT(Ctrl + C) 시그널을 받으면 핸들러 함수 실행
테스트 : Ctrl + C를 여러번 눌러보세요

*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int signum)
{
    printf("\n");
    printf("============================\n");
    printf("SIGINT 시그널을 받았습니다!\n");
    printf("시그널 번호 : %d\n", signum);
    printf("============================\n");
    printf("\n");
}

int main()
{
    int count = 0;
    signal(SIGINT, sigint_handler);
    
    printf("SIGINT 핸들러를 등록했습니다. \n");
    printf("Ctrl + C를 눌러보세요! \n\n");

    while(1){
        printf("실행중 ... %d초 경과\n", count);
        sleep(1);
        count++;

        if (count >= 10){
            printf("10초 경과, 프로그램 종료.\n");
            exit(0);
        }
    }

    return 0;
}
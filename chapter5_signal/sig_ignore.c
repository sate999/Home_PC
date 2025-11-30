/*
sig_ignore.c
목적 : SIGINT(Ctrl + C) 시그널을 무시하는 예제

종료 방법 : Ctrl + \ (SIGQUIT) 또는 다른 터미널에서 kill
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main()
{
    signal(SIGINT, SIG_IGN);

    printf("SIGINT(Ctrl + C)를 무시하도록 설정했습니다.\n");
    printf("Ctrl + C를 눌러보세요. 종료되지 않습니다!\n");
    printf("종료하려면 Ctrl + \\를 누르세요.\n\n");

    int count = 0;
    while(1)
    {
        printf("실행 중 ... %d초 경과\n", count);
        sleep(1);
        count++;
    }
    
    return 0;
}
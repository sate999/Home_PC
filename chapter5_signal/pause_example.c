#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void func(int signum)
{
  fprintf(stderr, "in handler.\n");
}

int main()
{
  int i;
  signal(SIGALRM, func);
  alarm(1);
  i = pause();
  fprintf(stderr, "i = [%d]\n", i);
}
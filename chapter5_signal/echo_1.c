#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void sigint_signalhandler(int);

int main()
{
  int size = 0;
  char temp[BUFSIZ];

  signal(SIGINT, sigint_signalhandler);

  while(1)
  {
    size = read(0, temp, BUFSIZ);
    write(1, temp, size);
  }
}

void sigint_signalhandler(int sig)
{
  printf("SIGINT Received. [%d]\n", sig);
  exit(1);
}
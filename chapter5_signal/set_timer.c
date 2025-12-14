#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

void sigalrm_signalhandler(int);

int call_count = 0;

int main()
{
  struct itimerval t_val;

  signal(SIGALRM, sigalrm_signalhandler);

  t_val.it_value.tv_sec = 5L;
  t_val.it_value.tv_usec = 0L;
  t_val.it_interval.tv_sec = 1L;
  t_val.it_interval.tv_usec = 0L;

  setitimer(ITIMER_REAL, &t_val, NULL);

  while(call_count <= 5)
    ;

  printf("Terminate process!!!\n");
}

void sigalrm_signalhandler(int sig)
{
  call_count++;
  printf("SIGALRM received. count = %d\n", call_count);
}
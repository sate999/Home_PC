#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MSGSIZE 20

int main ()
{
  int fd[2], pid;
  char msgin[MSGSIZE], msgout[MSGSIZE] = "\nHello, world\n";

  if (pipe(fd) == -1)
  {
    fprintf(stderr, "error\n");
    exit(1);
  }
  
  if((pid = fork()) > 0)
  {
    close (fd[0]);
    write (fd[1], msgout, MSGSIZE);
  }
  else if (pid == 0)
  {
    close (fd[1]);
    read (fd[0], msgin, MSGSIZE);
    puts (msgin);
  }
}
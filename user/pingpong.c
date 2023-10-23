#include "kernel/types.h"
#include "user.h"

static char *msg_ping = "ping";
static char *msg_pong = "pong";

void 
parent_stuff(int pipe[]) 
{
  char msg_buff[5];

  int w = write(pipe[1], msg_ping, 5);
  if (w != 5) printf("error while write (have written %d)\n", w);

  int r = read(pipe[0], msg_buff, 5);
  if (r <= 0) printf("error while reading (have read %d)\n", r);

  printf("(parent) %d: got %s\n", getpid(), msg_buff);
}

void 
child_stuff(int pipe[]) 
{
  char msg_buff[5];

  int r = read(pipe[0], msg_buff, 5);
  if (r <= 0) printf("error while reading (have read %d)\n", r);

  printf("(child) %d: got %s\n", getpid(), msg_buff);

  int w = write(pipe[1], msg_pong, 5);
  if (w != 5) printf("error while write (have written %d)\n", w);
}

int 
main(int argc, char *argv[]) 
{
  int p[2];
  pipe(p);

  int pid = fork();

  if (pid == 0) {
    parent_stuff(p);
  } else if (pid > 0) {
    child_stuff(p);
    wait((int *)0);
  } else {
    printf("fork failed");

    close(p[0]);
    close(p[1]);
    exit(-1);
  }

  close(p[0]);
  close(p[1]);
  exit(0);
}

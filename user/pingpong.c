#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int pipe_p[2]; // for parent
  int pipe_c[2]; // for child
  if (pipe(pipe_p) < 0) {
    fprintf(2, "pipe parent error.");
    exit(1);
  }
  if (pipe(pipe_c) < 0) {
    fprintf(2, "pipe child error.");
    exit(1);
  }

  int pid = fork();
  if (pid > 0) {
    close(pipe_c[1]); // unused

    close(pipe_p[0]);
    write(pipe_p[1], "x", 1);

    wait((int *)0);

    char buf1[1];
    read(pipe_c[0], buf1, sizeof(buf1));

    printf("%d: received pong\n", getpid());
  } else if (pid == 0) {
    close(pipe_p[1]); // unused
    close(pipe_c[0]);

    char buf2[1];
    read(pipe_p[0], buf2, sizeof(buf2));
    printf("%d: received ping\n", getpid());

    write(pipe_c[1], buf2, 1);
  } else {
    fprintf(2, "Fork failed\n");
    exit(1);
  }

  exit(0);
}

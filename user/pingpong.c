#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int parent_fd[2]; // for parent to send data to child
  int child_fd[2]; // for child to send data to parent
  if (pipe(parent_fd) < 0) {
    fprintf(2, "parent pipe error\n");
    exit(1);
  }
  if (pipe(child_fd) < 0) {
    fprintf(2, "child pipe error\n");
    exit(1);
  }

  int pid = fork();
  if (pid > 0) {
    // parent write should close the read end first
    close(parent_fd[0]);
    write(parent_fd[1], "x", 1);
    close(parent_fd[1]); // close the write end after use

    wait((int *)0);

    // parent read
    close(child_fd[1]);
    char buf1[1];
    read(child_fd[0], buf1, sizeof(buf1));
    close(parent_fd[0]);

    printf("%d: received pong\n", getpid());
  } else if (pid == 0) {
    // child read should close the write end
    close(parent_fd[1]);
    char buf2[1];
    read(parent_fd[0], buf2, sizeof(buf2));
    close(parent_fd[0]);

    printf("%d: received ping\n", getpid());

    // child write
    close(child_fd[0]);
    write(child_fd[1], buf2, 1);
    close(child_fd[1]);
  } else {
    fprintf(2, "Fork failed\n");
    exit(1);
  }

  exit(0);
}

#include "kernel/types.h"
#include "user/user.h"

void primes(int) __attribute__((noreturn));

int main(void) {
  int fd[2];
  if (pipe(fd) < 0) {
    fprintf(2, "main pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if (pid > 0) {
    // parent
    close(fd[0]);
    for (int i = 2; i <= 280; ++i) {
      write(fd[1], &i, sizeof(i));
    }
    close(fd[1]);
    wait(0);
  } else if (pid == 0) {
    // child
    close(fd[1]);
    primes(fd[0]);
  } else {
    fprintf(2, "main fork failed\n");
    exit(1);
  }

  exit(0);
}

void primes(int in_fd) {
  int p; // the prime that divides n
  int bytes = read(in_fd, &p, sizeof(p));
  if (bytes <= 0) {
    exit(0);
  }

  printf("prime %d\n", p);

  int n; // the number is divided.
  int pipe_fd[2];
  int spawned = 0;

  while ((bytes = read(in_fd, &n, sizeof(n))) > 0) {
    if (n % p != 0) {
      if (spawned == 0) {
        spawned = 1;

        if (pipe(pipe_fd) < 0) {
          fprintf(2, "child pipe failed, n: %d, p: %d\n", n, p);
          exit(1);
        }

        int pid = fork();
        if (pid > 0) {
          // parent
          close(pipe_fd[0]);
          write(pipe_fd[1], &n, sizeof(n));
        } else if (pid == 0) {
          // child
          close(in_fd);
          close(pipe_fd[1]);
          primes(pipe_fd[0]);
          // remember to jump out of the loop in child process.
          exit(0);
        } else {
          fprintf(2, "child fork failed, n: %d, p: %d\n", n, p);
          exit(1);
        }
      } else {
        write(pipe_fd[1], &n, sizeof(n));
      }
    }
  }

  close(in_fd);
  close(pipe_fd[1]);
  wait(0);
  exit(0);
}

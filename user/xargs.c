#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "xargs: missing arguments.\n");
    exit(1);
  }

  char str[128];
  for (;;) {
    gets(str, 128);
    int len = strlen(str);
    if (len > 0) {
      int pid = fork();
      if (pid == 0) {
        char *args[argc + 1];
        int n = 0;

        for (int i = 1; i < argc; ++i) {
          args[n++] = argv[i];
        }
        str[len - 1] = '\0'; // remove the final '\n' read from gets()
        args[n++] = str;
        args[n] = 0;

        exec(args[0], args);
      } else if (pid > 0) {
        wait(0);
      } else {
        fprintf(2, "xargs: fork failed.\n");
        exit(1);
      }
    } else {
      break;
    }
  }

  return 0;
}

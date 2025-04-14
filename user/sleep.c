#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: sleep [number of clock ticks]...\n");
    exit(1);
  }

  const char *s = argv[1];
  const int ticks = atoi(s);

  sleep(ticks);
  exit(0);
}

#include <stdio.h>
#include <unistd.h>

#include "xtilez.h"

void exec_process(KeybindingArgs *args) {
  if (fork() == 0) {
    setsid();
    execvp(args->cmd[0], args->cmd);
    perror(" failed");
    exit(EXIT_SUCCESS);
  }
}

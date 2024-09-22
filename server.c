#include "sc/socket/sc_sock.h"
#include <stdio.h>

void run_app() {}

int main(int argn, char **argv) {

  sc_sock_startup();

  puts("Hello");
  run_app();

  sc_sock_cleanup();
  return 0;
}

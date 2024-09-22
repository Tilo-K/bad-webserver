#include "sc/socket/sc_sock.h"
#include <stdio.h>
#include <stdlib.h>

void run_app() {
  char *host = "0.0.0.0";
  char *port = "8080";

  struct sc_sock *socket = malloc(sizeof(struct sc_sock));
  sc_sock_init(socket, 0, true, SC_SOCK_INET);

  int error = sc_sock_listen(socket, host, port);
  if (error != 0) {
    puts(sc_sock_error(socket));
  }

  sc_sock_term(socket);
  free(socket);
}

int main(int argn, char **argv) {

  sc_sock_startup();

  puts("Hello");
  run_app();

  sc_sock_cleanup();
  return 0;
}

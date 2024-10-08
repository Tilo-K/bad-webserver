#include "handle.c"
#include "sc/mutex/sc_mutex.h"
#include "sc/queue/sc_queue.h"
#include "sc/socket/sc_sock.h"
#include "sc/thread/sc_thread.h"
#include <stdio.h>
#include <stdlib.h>

void run_app() {
  char *host = "0.0.0.0";
  char *port = "8080";

  struct sc_queue_ptr conn_queue;
  sc_queue_init(&conn_queue);

  struct sc_mutex queue_mutex;
  sc_mutex_init(&queue_mutex);

  struct sc_sock *socket = malloc(sizeof(struct sc_sock));
  sc_sock_init(socket, 0, true, SC_SOCK_INET);

  struct HandleClientArgs args = {.queue = &conn_queue, .mutex = &queue_mutex};

  struct sc_thread threads[10];
  for (int i = 0; i < 10; i++) {
    sc_thread_init(&threads[i]);
    sc_thread_start(&threads[i], handle_client, &args);
  }

  puts("Started threads");

  int error = sc_sock_listen(socket, host, port);
  if (error != 0) {
    puts(sc_sock_error(socket));
  }

  while (true) {
    struct sc_sock *client = malloc(sizeof(struct sc_sock));

    error = sc_sock_accept(socket, client);
    if (error != 0) {
      puts(sc_sock_error(socket));
    }

    char buff[1024];
    sc_sock_print(socket, buff, 1024);

    printf("Accepted client: %s\n", buff);

    sc_mutex_lock(&queue_mutex);
    sc_queue_add_last(&conn_queue, client);
    sc_mutex_unlock(&queue_mutex);
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

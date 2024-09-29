#include "sc/mutex/sc_mutex.h"
#include "sc/queue/sc_queue.h"
#include "sc/socket/sc_sock.h"
#include "sc/time/sc_time.h"
#include <stdio.h>
#include <stdlib.h>

struct HandleClientArgs {
  struct sc_queue_ptr *queue;
  struct sc_mutex *mutex;
};

void *handle_client(void *args) {
  struct HandleClientArgs *cargs = args;

  struct sc_sock *client = NULL;

  while (true) {
    sc_mutex_lock(cargs->mutex);
    if (sc_queue_empty(cargs->queue)) {
      client = NULL;
    } else {
      client = sc_queue_del_last(cargs->queue);
    }
    sc_mutex_unlock(cargs->mutex);

    if (client == NULL) {
      sc_time_sleep(1); // TODO: Evaluate this. Maybe sleep longer?
      continue;
    }

    printf("Got a client! %p\n", client);

    free(client);

    puts("End of client communication\n");
  }
  return NULL;
}

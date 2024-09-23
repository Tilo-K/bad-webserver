#include "sc/mutex/sc_mutex.h"
#include "sc/queue/sc_queue.h"
#include "sc/socket/sc_sock.h"

struct HandleClientArgs {
  struct sc_queue_ptr *queue;
  struct sc_mutex *mutex;
};

void *handle_client(void *args) {
  struct HandleClientArgs *cargs = args;
  return NULL;
}

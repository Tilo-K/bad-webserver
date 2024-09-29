#include "sc/mutex/sc_mutex.h"
#include "sc/queue/sc_queue.h"
#include "sc/socket/sc_sock.h"
#include "sc/string/sc_str.h"
#include "sc/time/sc_time.h"
#include <stdio.h>
#include <stdlib.h>

struct HandleClientArgs {
  struct sc_queue_ptr *queue;
  struct sc_mutex *mutex;
};

struct Header {
  char *key;
  char *value;
};

struct Headers {
  char *verb;
  char *path;
  char *version;

  struct Header **headers;
};

struct Headers parse_headers(char *request_data, int data_length) {
  puts("Parsing headers\n");
  char last_char = '0';
  int header_length = -1;

  struct Headers *headers = malloc(sizeof(struct Headers));

  for (int i = 0; i < data_length; i++) {
    char curr_char = request_data[i];

    if (curr_char == '\n' && last_char == '\n') {
      header_length = i - 1;
      break;
    }
    if (curr_char != '\r') {
      last_char = curr_char;
    }
  }

  int number_of_headers = 0;
  for (int i = 0; i <= header_length; i++) {
    if (request_data[i] == '\n')
      number_of_headers++;
  }

  printf("Found %d headers\n", number_of_headers);

  struct Header **headerItems =
      malloc(sizeof(struct Header *) * number_of_headers);

  char *save = NULL;
  char *line;

  int header_idx = 0;
  int line_num = 0;
  while ((line = sc_str_token_begin(sc_str_create(request_data), &save,
                                    "\n")) != NULL) {
    printf("Parsing header index: %d\n", header_idx);
    line_num++;
    if (line_num == 1) {
      int curr = 0;
      char *save2 = NULL;
      char *part;
      while ((part = sc_str_token_begin(sc_str_create(line), &save2, " ")) !=
             NULL) {

        printf("Line one part: %s\n", part);
        switch (curr) {
        case 0:
          headers->verb = part;
          break;
        case 1:
          headers->path = part;
          break;
        case 2:
          headers->version = part;
          break;
        }
        curr++;
      }
      continue;
    }

    puts("Starting to parse normal headers\n");
    char *key = NULL;
    char *value = NULL;
    char *part;
    char *save2 = NULL;

    sc_str_trim(&line, "\t \n:");

    printf("Line is: %s\n", line);
    while ((part = sc_str_token_begin(sc_str_create(line), &save2, ": ")) !=
           NULL) {
      printf("Parsing part: %s", part);
      if (key != NULL && value != NULL) {
        key = NULL;
        value = NULL;
      }
      if (key == NULL) {
        key = part;
        continue;
      }
      value = part;
    }

    struct Header *head = malloc(sizeof(struct Header));
    puts("Before setting head\n");
    headerItems[header_idx] = head;
    puts("After setting head\n");
    header_idx++;
  }

  headers->headers = headerItems;

  return *headers;
}

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

    char buffer[1024 * 1024];
    int received = sc_sock_recv(client, buffer, 1024 * 1024, 0);

    printf("Request:\n%s", buffer);

    struct Headers headers = parse_headers(buffer, received);

    free(client);

    puts("End of client communication\n");
  }
  return NULL;
}

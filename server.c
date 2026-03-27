#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

struct client_info {
  int cfd;
  int client_id;
  bool quitted;
};

int total_message_count = 0;
int client_id_counter = 1;

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info client_list[128];

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  int client_cfd = client->cfd;
  int client_id = client->client_id;
  client = NULL;

  size_t sentence_len = 0;
  char sentence_buffer[BUF_SIZE];

  while (sentence_len = read(client_cfd, sentence_buffer, BUF_SIZE)) {
    sentence_buffer[sentence_len] = '\0';

    pthread_mutex_lock(&count_mutex);
    total_message_count++;
    pthread_mutex_unlock(&count_mutex);

    for (int i = 0; i < client_id_counter; i++) {
      send(client_list[i].cfd, sentence_buffer, sentence_len, 0);
    }

    printf("Client %d: %s\n", client_id, sentence_buffer);
  }

  if (close(client_cfd) != -1) {
    printf("Client %d quit the chat\n", client_id);
    return NULL;
  } else {
    handle_error("server: client quit failed");
  }
  return NULL;
}

int main(int argc, char *argv[]) {

  // init port number and no. of max clients
  if (argc == 3) {
    long lport = atoi(argv[1]);
    const int PORT = (int)lport;
    long lmaxclient = atoi(argv[2]);
    const int MAX_CLIENT = (int)lmaxclient;

    pthread_t client_threads[MAX_CLIENT];

    struct sockaddr_in addr;
    int sfd;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    int client_file = 0;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
      handle_error("server: TCP socket failed");
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
      handle_error("server: bind error");
    }

    if (listen(sfd, LISTEN_BACKLOG) == -1) {
      handle_error("server: listen error");
    }

    while (1) {
      client_file = accept(sfd, (struct sockaddr *)&addr, &addr_len);

      if (client_file == -1) {
        handle_error("server: accept error");
      } else if (client_file > 0 && client_id_counter < MAX_CLIENT) {
        printf("client %d accepted\n", client_id_counter);
        client_list[client_id_counter - 1].cfd = client_file;
        client_list[client_id_counter - 1].client_id = client_id_counter;
        client_list[client_id_counter - 1].quitted = false;

        pthread_create(&client_threads[client_id_counter - 1], NULL,
                       handle_client, &client_list[client_id_counter - 1]);
        client_id_counter++;
        pthread_detach(client_threads[client_id_counter - 2]);
      }
    }

    if (close(sfd) == -1) {
      handle_error("server: close failed");
    }

    return 0;
  } else {
    printf(
        "The syntax for server is : ./server <port number> <no. of clients>\n");
    return 0;
  }
}

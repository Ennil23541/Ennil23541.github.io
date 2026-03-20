/*
1. What is the address of the server it is trying to connect to (IP address and
port number)
IP address = "127.0.0.1", port number = 8000

2. Is it UDP or TCP? How do you know?
TCP, "man socket" said that the second parameter "SOCK_STREAM" sets a sequenced
communication.

3. The client is going to send some data to the server. Where does it get this
data from? How can you tell in the code?
It reads from stdin, as the while loop runs with read from STDIN_FILENO

4. How does the client program end? How can you tell that in the code?
By inputing a message of size 1 byte in stdin(just press enter), so the while
condition of read size bigger than 1 would be false because there is exact 1
byte sent to the buffer, and it is not an error
*/
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUF_SIZE 64
#define ADDR "127.0.0.1"

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int main() {
  struct sockaddr_in addr; // IP address?
  int sfd;                 // socket connection id
  ssize_t num_read;        // number of words in a message
  char buf[BUF_SIZE];      // buffer for each message

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in)); // init the sockaddr_in at &addr
                                                // by filling it with 0s
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  // connect sfd socket to address stored in addr, length = size of addr
  int res = connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if (res == -1) {
    handle_error("connect");
  }

  // message sending process
  while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) >
         1) { // while there's words in stdin
    if (write(sfd, buf, num_read) != num_read) {
      handle_error("write");
    }
    printf("Just sent %zd bytes.\n", num_read);
  }

  if (num_read == -1) {
    handle_error("read");
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t client_threads[32];

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  // move all data here and free the pointer
  int client_cfd = client->cfd;
  int client_id = client->client_id;
  client = NULL;

  size_t sentence_len = 0;
  char sentence_buffer[BUF_SIZE];

  // TODO: print the message received from client
  // TODO: increase total_message_count per message
  while (sentence_len = read(client_cfd, sentence_buffer, BUF_SIZE)) {
    sentence_buffer[sentence_len] =
        '\0'; // prevent printing gibberish left from last message

    pthread_mutex_lock(&count_mutex);
    total_message_count++;
    pthread_mutex_unlock(&count_mutex);

    printf("Client %d's message here: %s\n", client_id, sentence_buffer);
    printf("This is message %d from client %d\n", total_message_count,
           client_id);
  }

  if (close(client_cfd) != -1) {
    printf("client %d quitted\n", client_id);
    return NULL;
  }
  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  socklen_t addr_len = sizeof(struct sockaddr_in);

  int client_file = 0;
  struct client_info client_list[32];

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    // TODO: create a new thread when a new connection is encountered
    // TODO: call handle_client() when launching a new thread, and provide
    // client_info

    client_file = accept(sfd, (struct sockaddr *)&addr, &addr_len);

    if (client_file == -1) {
      handle_error("accept");
    } else if (client_file > 0) {
      printf("client %d accepted\n", client_id_counter);
      client_list[client_id_counter - 1].cfd = client_file;
      client_list[client_id_counter - 1].client_id = client_id_counter;
      pthread_create(&client_threads[client_id_counter - 1], NULL,
                     handle_client, &client_list[client_id_counter - 1]);
      client_id_counter++;
      pthread_detach(client_threads[client_id_counter - 2]);
    }
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}

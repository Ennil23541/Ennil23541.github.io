#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// #define PORT 8000
#define BUF_SIZE 256
// #define ADDR "127.0.0.1"

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int main(int argc, char *argv[]) {
  struct sockaddr_in addr;
  int sfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  // init port number and IP address
  if (argc == 5) {
    long lport = atoi(argv[2]);
    const int PORT = (int)lport;
    char *ADDR = argv[1];
    int number_of_messages = atoi(argv[3]);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
      handle_error("TCP socket failed");
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
      handle_error("inet_pton failed");
    }

    int res =
        connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    if (res == -1) {
      handle_error("connection failed");
    }

    // init message
    char *message_list[number_of_messages];
    for (int i = 0; i < number_of_messages; i++) {
    
      message_list[i] = "Message\n\0";
    }


    /*char buffer[1024];
//    while (read(sfd, buffer, 1024)) {
      while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) {
        if (write(sfd, buf, num_read) != num_read) {
          handle_error("write failed");
        }

        printf("Sent %zd bytes\n", num_read);
        if(read(sfd,buffer,1024)){

          printf("From server: %s\n", buffer);
        }
      }
//    }
*/

    // char message_type[1] = "0";
    for (int i = 0; i < number_of_messages; i++) {
      char send_message[BUF_SIZE];
      send_message[0] = '0';
      int count = 0;
      while (message_list[i][count] != '\0') {
        send_message[count + 1] = message_list[i][count];
        count++;
      }
      send_message[count+1]='\0';

      strncpy(buf, send_message, BUF_SIZE);
      if (write(sfd, buf, BUF_SIZE) == -1) {
        handle_error("write");
      } else {
        printf("Sent: %s\n", send_message);
      }
      // message_type[1] = '\0';
    }
    close(sfd);
    printf("all sent\n");
    exit(EXIT_SUCCESS);
  } else {
    printf("client usage: ./client <IP address> <port number> <number of "
           "messages> <log file path>");
  }
  // return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define MYPORT "8080"
#define BACKLOG 10

int main() {

  printf("Demiurge: A C server from scratch.\n");

  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sockfd, new_fd, status;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, MYPORT, &hints, &res)) != 0) {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(EXIT_FAILURE);
  }

  // Get socket file descriptor
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd < 0) {
      perror("socket");

      freeaddrinfo(res);
      exit(EXIT_FAILURE);
  }

  printf("Socket opened successfully with the file descriptor: %d\n", sockfd);

  // Bind socket
  if (bind(sockfd, res->ai_addr, res->ai_addrlen)) {
      perror("bind");

      close(sockfd);
      freeaddrinfo(res);
      exit(EXIT_FAILURE);
  } else {
      printf("Binding complete!");
  }

  // Listen on port
  if (listen(sockfd, BACKLOG)) {
      perror("listen");

      close(sockfd);
      freeaddrinfo(res);
      exit(EXIT_FAILURE);
  } else {
      printf("Listening...");
  }

  // Accept incoming request
  addr_size = sizeof their_addr;
  new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);


  close(sockfd);
  freeaddrinfo(res);

  exit(EXIT_SUCCESS);
}

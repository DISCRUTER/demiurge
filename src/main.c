#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490" // Prot user will connect in
#define BACKLOG 10  // Allowed limit for pending request queue

/*
 * Handle SIGCHLD signal
 * Reaps all the child process which have exited.
*/
void sigchld_handler(int s) {
    (void)s;                               // Silent the signal int
    int saved_errno = errno;               // Save errno to prevent modification

    while(waitpid(-1, NULL, WNOHANG) > 0); // Reap any completed child process

    errno = saved_errno;
}


/*
 * Get IP4 or IP6 addr from the connection request.
 */
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main() {

  printf("Demiurge: A C server from scratch.\n");

  // Socket opeining and addrinfo
  struct addrinfo hints, *res, *p;
  int sockfd, new_fd;
  int status;
  // Listening to connection
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  // Signal Handling
  struct sigaction sa;
  // Network to Presentation
  char s[INET6_ADDRSTRLEN];
  // Socket option
  int opt_val = 1;


  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(EXIT_FAILURE);
  }

  for (p = res; p != NULL; p = p->ai_next) {
      // Get socket file descriptor
      if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
          perror("server: socket");
          continue;
      }

      // Setting socket options
      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1) {
          perror("setsocket");
          exit(EXIT_FAILURE);
      }

      // Bind the port
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
          close(sockfd);
          perror("server: bind");
          continue;
      }

      break;
  }

  freeaddrinfo(res);  // Free addrinfo memeory

  // Server didn't bind to any particular address
  if (p == NULL) {
      fprintf(stderr, "Server failed to bind\n");
      exit(EXIT_FAILURE);
  }

  // Listen on port
  if (listen(sockfd, BACKLOG)) {
      perror("listen");
      exit(EXIT_FAILURE);
  } else {
      printf("Server listening on %s...\n", PORT);
  }

  // Setting up SIGCHLD handler
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  // Registering handler with SIGCHLD signal
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
  }

  printf("Server waiting for connections...\n");

  // accept() loop
  while (1) {
      // Accept incoming request
      addr_size = sizeof their_addr;
      new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
      if (new_fd == -1) {
          perror("accept");
          continue;
      }

      inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr), s, sizeof s);
      printf("server: got connection from %s\n", s);

      if (!fork()) {     // inside the child proc
          close(sockfd); // child proc does not need the listener
          if (send(new_fd, "Hello World", 13, 0) == -1) {
              perror("send");
          }
          close(new_fd);
          exit(EXIT_SUCCESS);
      }
      close(new_fd);  // Parent doesn't need this
  }

  exit(EXIT_SUCCESS);
}

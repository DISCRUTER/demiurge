#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define PORT "3490"      // Server port
#define MAXDATASIZE 100  // recv buffer size


// Change sockaddr to ip equiv
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {

    // addrinfo
    struct addrinfo hints, *res, *p;
    int status;
    // Socket
    int sockfd;
    // recv bytes
    int numbytes;
    char buf[MAXDATASIZE];
    // network to presentation
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr, "usage: client hostname.\n");
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    for (p = res; p != NULL; p = p->ai_next) {
        // Socket FD
        if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)p->ai_addr), s, sizeof(s));
        printf("Attempting to connect to %s\n", s);

        // Connect to server
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
            perror("client: connect");
            close(sockfd);
            continue;
        }

        break;
    }

    // Check for failure
    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(2);
    }

    printf("Successfully connected to %s\n", s);

    freeaddrinfo(res); // Free up addrinfo

    // Recieve the data
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    buf[numbytes] = '\0';

    printf("client: received '%s'\n", buf);

    close(sockfd);

    exit(EXIT_SUCCESS);
}

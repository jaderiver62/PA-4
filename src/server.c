#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server.h"

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024 


void *clientHandler(void *socket) {

    // Receive packets from the client

    // Determine the packet operatation and flags

    // Receive the image data using the size

    // Process the image data based on the set of flags

    // Acknowledge the request and return the processed image data
}

int main(int argc, char* argv[]) {

    // Creating socket file descriptor
    int listen_fd, conn_fd;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
        perror("socket error");

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind the socket to the port
    int ret = bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if(ret == -1)
        perror("bind error");

    // Listen on the socket
    ret = listen(listen_fd, MAX_CLIENTS);
    if(ret == -1)
        perror("listen error");

    // Accept connections and create the client handling threads

    // struct sockaddr_in clientaddr;
    // socklen_t clientaddr_len = sizeof(clientaddr);
    // conn_fd = accept(listen_fd, (struct sockaddr *) &clientaddr, &clientaddr_len);
    // if(conn_fd == -1)
    //     perror("accept error");

    // Release any resources

    return 0;
}

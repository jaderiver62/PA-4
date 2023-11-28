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
///// Receive packets from the client
    packet_t packet;
    int conn_fd = *(int *) socket;
    int bytes_read;

    bytes_read = recv(conn_fd, &packet, BUFFER_SIZE, 0);
    if(bytes_read <= 0)
        perror("recv error");

    // typedef struct packet {
    //     unsigned char operation : 4;
    //     unsigned char flags : 4;
    //     unsigned int size;
    //     unsigned char checksum[SHA256_BLOCK_SIZE];
    // } packet_t; 

///// Determine the packet operatation and flags

///// Receive the image data using the size

///// Process the image data based on the set of flags - ROTATE THE IMAGE

///// Acknowledge the request and return the processed image data

    // typedef struct processing_args {
    //     int number_worker;
    //     char *file_name;
    // } processing_args_t;

}

int main(int argc, char* argv[]) {

///// Creating socket file descriptor
    int listen_fd, conn_fd;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == INVALID)
        perror("socket error");

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

///// Bind the socket to the port
    int ret = bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if(ret == INVALID)
        perror("bind error");

///// Listen on the socket
    ret = listen(listen_fd, MAX_CLIENTS);
    if(ret == INVALID)
        perror("listen error");

///// Accept connections and create the client handling threads
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    conn_fd = accept(listen_fd, (struct sockaddr *) &clientaddr, &clientaddr_len);
    if(conn_fd == -1)
        perror("accept error");

    // Spawn a client handling thread
    pthread_t client_thread;
    pthread_create(&client_thread, NULL, clientHandler, (void *) &conn_fd);

///// After receiving IMG_OP_EXIT operation message from client, close the connection and clean up resources.


///// Release any resources
    close(listen_fd);
    close(conn_fd);


    return 0;
}

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
    unsigned char *image_data;
    image_data = malloc(packet.size);
    if (image_data == NULL) {
        perror("malloc error");
        // Handle errors
        return;
    }

    bytes_read = recv(conn_fd, &packet, BUFFER_SIZE, 0);
    if(bytes_read <= 0)
        perror("recv error");

    int bytes_read;
    bytes_read = recv(conn_fd, &packet, BUFFER_SIZE, 0);
    if(bytes_read <= 0)
        perror("recv error");

///// Determine the packet operation and flags
    int operation = packet.operation;
    int flags = packet.flags;
    int size = packet.size;
    char *checksum = packet.checksum;

    if(operation == IMG_OP_EXIT) {
        // Close the connection
        close(conn_fd);
        // Clean up resources
        return NULL;
    }
    else if(operation == IMG_OP_ROTATE) {
        // Rotate the image

///// Receive the image data using the size
        bytes_read = recv(conn_fd, image_data, packet.size, 0);
        if (bytes_read <= 0) {
            // Handle errors and free memory
            // Send back the package with an IMG_OP_NAK message if an error happens. Skip the rest of the job for this image.
            free(image_data);
            return;
        }

///// Process the image data based on the set of flags - ROTATE THE IMAGE
        if(flags == IMG_FLAG_ROTATE_180) {
            // Rotate 180
        }
        else if(flags == IMG_FLAG_ROTATE_270) {
            // Rotate 270
        }
        else {
            // Error
        }
    }
    else {
        // Invalid operation
    }

///// Acknowledge the request and return the processed image data
    packet_t response;
    response.operation = IMG_OP_ACK;
    response.flags = 0;
    response.size = 0;
    memset(response.checksum, 0, SHA256_BLOCK_SIZE);

    // Send the response packet
    if(send(conn_fd, &response, sizeof(response), 0) < 0) {
        perror("send error");
        // Handle errors
        return;
    }

    // Send the processed image data
    if(send(conn_fd, image_data, packet.size, 0) < 0) {
        perror("send error");
        // Handle errors
        return;
    }

    // Free the image data
    free(image_data);

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

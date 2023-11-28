// TODO: 
// Send a package with IMG_OP_ROTATE to the server and receive the package on the server side
// Utilize the request queue to send the images to the server
// Used labs as a reference for the client code


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <sys/resource.h>
#include "client.h"

#define PORT 8080
#define BUFFER_SIZE 1024 

int send_file(int socket, const char *filename) {
    // Open the file
    FILE* fileptr = fopen(filename, "r");
    // Set up the request packet for the server and send it
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    // Send the file data
    while(fgets(buffer, BUFFER_SIZE, fileptr) != NULL){
        send(socket, buffer, BUFFER_SIZE, 0);
    }
    // Close the file
    fclose(fileptr);

    return 0;
}

int receive_file(int socket, const char *filename) {
    // Open the file
    FILE* fileptr = fopen(filename, "w");
    // Receive response packet
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    // Receive the file data
    while(recv(socket, buffer, BUFFER_SIZE, 0) > 0){
        // Write the data to the file
        fputs(buffer, fileptr);
    }
    // Close the file
    fclose(fileptr);

    return 0;
}

int main(int argc, char* argv[]) {
    if(argc != 3){
        fprintf(stderr, "Usage: ./client File_Path_to_images File_Path_to_output_dir Rotation_angle. \n");
        return 1;
    }
    
    // Set up socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
        perror("socket error");

    // Connect the socket
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    int ret = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if(ret == -1)
        perror("connect error");
    // Read the directory for all the images to rotate
    DIR* dir = opendir(argv[1]);
    struct dirent* entry;
    // Check that the request was acknowledged
    while((entry = readdir(dir)) != NULL){
        // Send the image to the server
        send_file(sockfd, entry->d_name);
    }
    // Receive the processed image and save it in the output dir
    int ret = receive_file(sockfd, argv[2]);
    if(ret == -1)
        perror("receive error");
    // Terminate the connection once all images have been processed
    close(sockfd);
    // Release any resources
    closedir(dir);

    return 0;
}

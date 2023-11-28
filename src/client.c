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

int queue_size = 0;

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

    // Read command line arguments 
    char* image_dir = argv[1];
    char* output_dir = argv[2];
    int rotation_angle = atoi(argv[3]);

    // Create a request queue
    request_t request_queue[MAX_QUEUE_LEN];

////// Set up socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == INVALID)
        perror("socket error");

////// Connect the socket
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    int ret = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    
    // Check that the request was acknowledged
    if(ret == INVALID)
        perror("connect error");

///// Read the directory for all the images to rotate
    DIR* dir = opendir(image_dir);
    struct dirent* entry;

    // While there are files in the directory
    while((entry = readdir(dir)) != NULL){
        // Add to the request queue using the request_t struct-> rotation angle and file name
        request_queue->rotation_angle = rotation_angle;
        request_queue->file_name = entry->d_name;
        queue_size++;
    }

///// Send the image data to the server
    while(queue_size > 0){
        // Pop a file from the queue
        request_t request = request_queue[queue_size - 1];
        queue_size--;
        
        // Send a packet with the IMG_FLAG_ROTATE_XXX message header desired rotation Angle, Image size, and data.
        packet_t packet;
        packet.operation = IMG_OP_ROTATE;

        // Find IMG_FLAG_ROTATE_XXX using the rotation angle
        if(request.rotation_angle == 180){
            packet.flags = IMG_FLAG_ROTATE_180;
        }
        else if(request.rotation_angle == 270){
            packet.flags = IMG_FLAG_ROTATE_270;
        }
        else{
            packet.flags = 0;
        }
        packet.size = sizeof(request);
        // packet.checksum???

        // Send the packet   
        send(sockfd, &packet, sizeof(packet), 0);

        // Send the image file
        char* done_img = strcat(output_dir, request.file_name);

        // Send the file
        send_file(sockfd, request.file_name);

        // Receive the response packet
        packet_t response;
        recv(sockfd, &response, sizeof(response), 0);
        
////// Check that the response packet is valid
        if(response.operation == IMG_OP_ACK){
////// Receive the processed image and save it in the output dir
            receive_file(sockfd, done_img);
        }
        else if(response.operation == IMG_OP_NAK){
            // Log the error
            FILE* fileptr = fopen(LOG_FILE_NAME, "w");
            fprintf(fileptr, "Error: %s\n", strerror(errno));
            fclose(fileptr);
        }

    }

    // While request queue is not empty
        // Pop a file from the queue
        // Receive the processed image and save it in the output dir
            // Receive the response packet containing the processed image from the server
            // Save the image to the output directory
    // While request queue is empty
    // Send ‘terminate’ message through socket using the IMG_OP_EXIT operation
    if (send(sockfd, IMG_OP_EXIT, strlen(IMG_OP_EXIT), 0) == INVALID) {
        perror("terminate error");
    }

///// Terminate the connection once all images have been processed
    close(sockfd);
///// Release any resources
    closedir(dir);

    return 0;
}




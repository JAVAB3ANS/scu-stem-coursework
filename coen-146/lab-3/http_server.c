//  Created by Behnam Dezfouli
//  CSEN, Santa Clara University
// 
// This program implements a web server
//
// The input arguments are as follows:
// argv[1]: Server's port number
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <signal.h> 
#include <sys/stat.h> // to get file size

#define COMMAND_BUFF 15000 // Size of the buffer used to store the result of command execution
#define SOCK_READ_BUFF 4096 // Size of the buffer used to store the bytes read over socket
#define REPLY_BUFF 20000 // Size of the buffer used to store the message sent to client
#define FILE_READ 10

#define HTML_FILE "index.html"
// Socket descriptors
int socket_fd = 0; // socket descriptor
int connection_fd = 0; // new connection descriptor 
void INThandler(int sig) {
    char input;
    signal(sig, SIG_IGN);
    printf("Did you hit Ctrl-C?\n Do you want to quit? [y/n] ");
    input = scanf("%c", & input);
    
    if (input == 'y' || input == 'Y') {
        exit(0);
    } else {
        signal(SIGINT, INThandler);
    }

    getchar(); // Get new line character 
}

// main function ---------------
int main(int argc, char * argv[]) {
    // Register a function to handle SIGINT ( SIGNINT is interrupt the process )
    signal(SIGINT, INThandler);

    int net_bytes_read; // number of bytes received over socket
    struct sockaddr_in serv_addr; // Address format structure
    char net_buff[SOCK_READ_BUFF]; // buffer to hold characters read from socket
    char message_buff[REPLY_BUFF]; // buffer to hold the message to be sent to the client
    char file_buff[FILE_READ]; // to hold the bytes read from source file
    FILE * source_file; // pointer to the source file

    // pointer to the file that will include the received bytes over socket
    FILE * dest_file;

    if (argc < 2) { // Note: the name of the program is counted as an argument
        printf("Port number not specified!\n");
        return 1;
    }

    memset( & serv_addr, '\0', sizeof(serv_addr));
    memset(message_buff, '\0', sizeof(message_buff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    dest_file = fopen(net_buff, "w");

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error: Could not create socket! \n");
        return 1;
    }

    // To prevent "Address in use" error
    // The SO_REUSEADDR socket option, which explicitly allows a process to bind to a port which remains in TIME_WAIT
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, & (int) {
            1
        }, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }

    // bind it to all interfaces, on the specified port number
    bind(socket_fd, (struct sockaddr * ) & serv_addr, sizeof(serv_addr));
    // Accept up to 1 connections

    if (listen(socket_fd, 1) < 0) {
        perror("Listen failed!");
    }

    int n_socket_fd = accept(socket_fd, (struct sockaddr * ) NULL, NULL);
    if (n_socket_fd < 0) {
        printf("Error establishing connnection\n");
        return (1);
    }

    printf("Listening to incoming connections... \n");
    unsigned int option = 0; // Variable to hold user option
    printf("1: System network configuration \n2: Regular HTTP server\n");
    scanf("%u", & option);

    // The web server returns current processor and memory utilization
    if (option == 1) {
        printf("System network configuration (using ifconfig)... \n");
    }

    // The requested file is returned
    else if (option == 2) {
        printf("Regular server (only serving html files)... \n\n");
    } else
        printf("Invalid Option! \n\n");

    while (1) {
        // Accept incoming connection request from client
        connection_fd = accept(socket_fd, (struct sockaddr * ) & serv_addr, (socklen_t * ) & serv_addr);

        printf("Incoming connection: Accepted! \n");
        memset(net_buff, '\0', sizeof(net_buff));
        // Return system utilization info
        if (option == 1) {
            // run a command -- we run ifconfig here (you can try other commands as well)

        FILE * system_file_buffinfo = popen("ifconfig", "r");

        char command_buff[COMMAND_BUFF]; //buffer to hold the result of command executed successfully

        if (system_file_buffinfo == NULL) { // check if the command was executed
            successfully
            printf("Error: could not run command! \n");
            return 1;
        }

        // read the result of the command execution
        // write from systemfileinfo to commandbuff
        // at the start: add header

        char *http_header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 10000\n\n ";

        strncpy(message_buff, http_header, strlen(http_header));

        while (fgets(command_buff, sizeof(command_buff), system_file_buffinfo) ! = NULL) { // read the result of the command execution 
            write(connection_fd, command_buff, strlen(command_buff));
            printf("%s", command_buff); //print the result of the command 
            execution
        }

        // close the file
        pclose(system_file_buffinfo);

        shutdown(connection_fd, SHUT_RDWR);

        close(connection_fd);

    } else if (option == 2) {
        // To get the size of html file
        struct stat sbuf; /* file status */

        // make sure the file exists
        // HTML_FILE is index.html and is statically defined

        printf("Working -1\n");

        if (stat(HTML_FILE, & sbuf) < 0) {
            perror("File not found!");
            exit(EXIT_FAILURE);
        }

        printf("Working 0\n");

        // Open the source file
        source_file = fopen(HTML_FILE, "r");

        if (source_file == NULL) {
            printf("Error: could not open the source file!\n");
            return 1;
        } else {
            int pack_size;
            char * http_header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 10000\n\n";
            strncpy(message_buff, http_header, strlen(http_header));
            while (pack_size = fread(file_buff, 1, FILE_READ, source_file), pack_size > 0) {
                strncpy(message_buff + strlen(message_buff), file_buff, strlen(file_buff));
            }
            write(connection_fd, message_buff, strlen(message_buff));
            char * temp[5000];
            recv(connection_fd, temp, 5000, 0);
            printf("%s", temp);
            printf("Reply sent to the client!\n");
        }
        shutdown(connection_fd, SHUT_RDWR);
        close(connection_fd);
    }
}

close(socket_fd);
}
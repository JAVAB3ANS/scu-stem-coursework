//
//  Behnam Dezfouli
//  CSEN, Santa Clara University
//

// This program implements a client that connects to a server and transfers the bytes read from a file over the established connection
//
// The input arguments are as follows:
// argv[1]: Server's IP address
// argv[2]: Server's port number
// argv[3]: Source file
// argv[4]: Destination file at the server side which includes a copy of the source file
  
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <ws2def.h>
#include <windows.h>
#include <io.h>
//#include <sys/socket.h>
#include <sys/types.h>
//#include <netinet/in.h>
//#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//#include <arpa/inet.h>

#define SIZE 10

int main (int argc, char *argv[]) {
	int sockfd = 0; 										// socket descriptor
	char net_buff[SIZE]; 									// to hold the bytes read from socket
	char file_buff[SIZE]; 									// to hold the bytes read from source file
	struct sockaddr_in serv_addr; 							// server address structure
	int net_bytes_read; 									// number of bytes received over socket
	FILE *source_file; 										// pointer to the source file

	if (argc < 5) {
		printf ("Usage: ./%s <server ip address> <server port number> <source file> <destination file>\n", argv[0]);
		return 1;
	} else {
		printf ("Server IP address: %s	Server port number: %s	Source file: %s	Destination file: %s \n", argv[1], argv[2], argv[3], argv[4]);
	}

	// Make sure intial values are zero

	memset (net_buff, '0', sizeof (net_buff));
	memset (file_buff, '0', sizeof (file_buff));
	memset (&serv_addr, '0', sizeof (serv_addr));
  
	// Open the source file

	source_file = fopen (argv[3], "r");

	if (source_file == NULL){
		printf ("Error: Cannot open the source file %s for reading \n", argv[3]); 
		return 1;
	} else {
		printf ("Source file %s is opened for reading \n", argv[3]);
	}
	
	// Create a socket

	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0){
		printf ("Error: Could not create a socket \n");
		return 1;
	} else {
		printf ("Socket is created \n");
	}

	// Set the server address structure

	serv_addr.sin_family = AF_INET;

	// Convert the port number from string to integer

	serv_addr.sin_port = htons (atoi (argv[2]));

	// Convert the IP address from string to integer

	if (inet_pton (AF_INET, argv[1], &serv_addr.sin_addr) <= 0){
		printf ("Error: inet_pton error occured \n");
		return 1;
	} else {
		printf ("Server IP address is converted to integer \n");
	}

	// Connect to the server

	if (connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0){
		printf ("Error: Connect Failed \n");
		return 1;
	} else {
		printf ("Connected to the server \n");
	}

	// Read the source file and send the bytes over the socket

	while ((net_bytes_read = fread (file_buff, 1, SIZE, source_file)) > 0){
		if (send (sockfd, file_buff, net_bytes_read, 0) != net_bytes_read){
			printf ("Error: Send error occured \n");
			return 1;
		} else {
			printf ("Bytes are sent to the server \n");
		}
	}

	// Send the destination file name to the server

	if (send (sockfd, argv[4], strlen (argv[4]), 0) != strlen (argv[4])){
		printf ("Error: Send error occured \n");
		return 1;
	} else {
		printf ("Destination file name is sent to the server \n");
	}

	// Receive the bytes from the server and write them to the destination file

	while ((net_bytes_read = recv (sockfd, net_buff, SIZE, 0)) > 0) {
		if (fwrite (net_buff, 1, net_bytes_read, stdout) != net_bytes_read){
			printf ("Error: Write error occured \n");
			return 1;
		} else {
			printf ("Bytes are received from the server \n");
		}
	}

	if (net_bytes_read < 0){
		printf ("Error: Read error occured \n");
		return 1;
	} else {
		printf ("File is received from the server \n");
	}

	// Close the socket

	close (sockfd);

	// Close the source file

	fclose (source_file);

	return 0;
}
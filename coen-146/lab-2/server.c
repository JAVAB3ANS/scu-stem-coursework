//
//  Behnam Dezfouli
//  CSEN, Santa Clara University
//

// This program implements a server that accepts connection from a client and copies the received bytes to a file
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

int main (int argc, char *argv[]) {
	char    message[10] = "received!";  			// message to be sent to the client when the destination file name is received
	int     net_bytes_read;             			// number of bytes received over socket
	int     socket_fd = 0;              			// socket descriptor
	int     connection_fd = 0;          			// new connection descriptor
	struct  sockaddr_in serv_addr;      			// Address format structure
	char    net_buff[1024];             			// buffer to hold characters read from socket
	FILE    *dest_file;                 			// pointer to the file that will include the received bytes over socket

	if (argc < 2) {									// Note: the name of the program is counted as an argument
		printf ("Port number not specified!\n");
		return 1; 
	} else {
		printf ("Port number is %s \n", argv[1]);
	}

	memset (net_buff, '0', sizeof (net_buff));											// Make sure intial values are zero
	memset (&serv_addr, '0', sizeof (serv_addr));

	socket_fd = socket (AF_INET, SOCK_STREAM, 0);										// Create a socket

	if (socket_fd < 0) {
		printf ("Error: Cannot create a socket!");
		return 1;
	} else {
		printf ("Socket created successfully! \n");
	}

	serv_addr.sin_family = AF_INET; 													// Set the server address structure
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
 
	serv_addr.sin_port = htons (atoi (argv[1])); 										// Convert the port number from string to integer

	if (bind (socket_fd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) { 	// Bind the socket to the server address
		printf ("Error: Cannot bind the socket to the server address!	");
		return 1;
	} else {
		printf ("Socket is bound to the server address successfully! \n");
	}

	if (listen (socket_fd, 10) < 0) {													// Listen for connections
		printf ("Error: Cannot listen for connections!");
		return 1;
	} else {
		printf ("Listening for connections ... \n");
	}

	connection_fd = accept(socket_fd, (struct sockaddr *) NULL, NULL);					// Accept a connection

	if (connection_fd < 0) {
		printf ("Error: Cannot accept a connection!	");
		return 1;
	} else {
		printf ("Connection accepted successfully! \n");
	}

	memset (net_buff, '0', sizeof (net_buff));

	net_bytes_read = read(connection_fd, net_buff, sizeof (net_buff));				// Receive the destination file name
	net_buff[net_bytes_read]='\0';

	if (net_bytes_read < 0) {
		printf ("Error: Cannot receive the destination file name!	");
		return 1;
	} else {
		printf ("Destination file name is %d %s \n", net_bytes_read, net_buff);
	}
 
	dest_file = fopen (net_buff, "w");													// Open the destination file

	if (dest_file == NULL) {
		printf ("Error: Cannot open the destination file!	");
		return 1;
	} else {
		printf ("Destination file is opened successfully! \n");
	}

	if (send (connection_fd, message, strlen (message), 0) < 0) {					    // Send a message to the client to indicate that the destination file name is received
		printf ("Error: Cannot send a message to the client!	");
		return 1;
	} else {
		printf ("Message is sent to the client successfully! \n");
	}
 
	while (1) {																		    // Receive the bytes over socket and write them to the destination file
		net_bytes_read = recv (connection_fd, net_buff, sizeof (net_buff), 0);

		if (net_bytes_read < 0) {
			printf ("Error: Cannot receive the bytes over socket!	");
			return 1;
		} else {
			printf ("Bytes are received over socket successfully! \n");

			if (net_bytes_read == 0) {
				printf ("End of file is reached! \n");
			}

			printf ("Number of bytes received over socket is %d \n", net_bytes_read);
		}

		if (net_bytes_read == 0) {
			break;
		}

		fwrite (net_buff, 1, net_bytes_read, dest_file);
	}

	fclose (dest_file);																	// Close the destination file
 
	close (socket_fd);																	// Close the socket									
	
	return 0;
}
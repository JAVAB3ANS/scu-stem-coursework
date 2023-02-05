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
#include <sys/stat.h>

int socket_fd = 0;	
int connection_fd = 0;

int main (int argc, char *argv[]){
	struct	sockaddr_in serv_addr;
	char	message_buff[20000];

	memset (&serv_addr,   '\0', sizeof (serv_addr));
	memset (message_buff, '\0', sizeof (message_buff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons (8011);

	socket_fd = socket (AF_INET, SOCK_STREAM, 0);
	bind(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(socket_fd, 1);

	while (1){
		connection_fd = accept(socket_fd, (struct sockaddr *)&serv_addr, (socklen_t*)&serv_addr);

		strncpy(message_buff, "HTTP/1.1 Here I am\n", strlen("HTTP/1.1 Here I am\n"));
		strncpy(message_buff + strlen(message_buff), "Content-type: text/html\n", strlen("Content-type: text/html\n"));
		strncpy(message_buff + strlen(message_buff), "\n", strlen("\n"));
		strncpy(message_buff + strlen(message_buff), "<html><title>My first web server!</title>", strlen("<html><title>My first web server!</title>"));
		strncpy(message_buff + strlen(message_buff), "<body bgcolor=""ffffff"">Da da da ...</body></html>\n", strlen("<body bgcolor=""ffffff"">Da da da ...</body></html\n"));

		write(connection_fd , message_buff, strlen(message_buff));

		shutdown (connection_fd, SHUT_RDWR);
		close (connection_fd);
	}

	close (socket_fd);
}

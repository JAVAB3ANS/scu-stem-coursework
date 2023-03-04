// SCU COEN 146
//
// This program implements the client side of stop & wait protocol
// the client transfers a file to the server
//
//
// For the stop and wait protocolL, the assumptions are:
//      -- packet corruption, and packet loss


/*Checklist, what the solution needs to have (in progress...): 

   UDP protocol
   Using sequence numbers (0 or 1)
   Calculate checksum.
   Using select() for waiting (only for client.c).
   Client/Server uses ACK messages.
   Deal with duplicate ACK messages
   Use a random function for testing to simulate package losses and errors.
   */ 

#include "tfv1.h"
 
// global variables
int state = 0; // only two states: 0 and 1
int sock;
struct sockaddr_in serverAddr;
 
socklen_t addr_size;
 
// list of functions
int main (int, char *[]);
 
// to reliably send a chunk of data
void my_send (char *, int);
 
// part of the my_send() function
void recv_ack (PACKET *, int); 

// computes checksum using XOR of bytes
int calc_checksum (char *, int); 

int main (int argc, char *argv[]) {
   char    buff[SIZE];
   FILE    *fp; // file to be sent to the server
   int     r;
  
   if (argc != 5) {
       printf ("usage:usage: ./client port IP source_file dest_file\n");
       return 1;
   }
  
   // configure address
   memset((char *)serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(atoi(argv[1]));
   inet_pton(AF_INET, argv[2], &serverAddr.sin_addr.s_addr);
   addr_size = sizeof(serverAddr);
  
   // create UDP socket
   if ((sock = socket (PF_INET, SOCK_DGRAM, 0)) < 0) {
       printf ("socket error\n");
       return 1;
   }
   printf ("UDP socket has been created!\n");
  
   if ((fp = fopen (argv[3], "rb")) == NULL) {
       printf ("fopen error\n");
       return 1;
   }
   printf ("Source file opened successfully!\n"); 
 
   printf ("Sending file name..."); 

   my_send(argv[4], sizeof(argv[4] + 1)); 

   printf ("Done!\n");  
  
   printf ("Now sending source file contents..."); 

   printf ("Done!\n"); 
  
   // After transmitting the file, a packet with no data (len = 0) is sent to
   // notify the receiver that the file transfer has completed
   printf ("Informing the server about the completion of file transmission...\n");
    my_send (NULL, 0); // send a packet with no data

   printf ("Done!\n");
  
  
   fclose (fp);
   close (sock);
  
   return 0;
  
} 

//***********************
// sends data (of size nbbytes) reliably to the sender
// **********************
void my_send (char *data, int nbytes) 
{
   PACKET  buf;
   int     r;
  
   buf.header.seq_ack = state;
   buf.header.len = nbytes;
   memcpy(buf.data, data, nbytes);
  
   //default checksum value is 0
   buf.header.checksum = 0;
  
   // simulating erroneous channel... corruption and loss
   // the probability of correct checksum is 80%
   r = rand () % 10;
   if (r <= 8) {
       buf.header.checksum = calc_checksum ((char *)&buf, sizeof (HEADER) + nbytes);
   } else {
       printf ("Packet got corrupted on the way!\n");
   }

   // the probability of no packet loss is 80%
   buf.header.checksum = calc_checksum ((char *)&buf, sizeof (HEADER) + nbytes);
   sendto(sock, &buf, sizeof(HEADER)+nbytes, 0, (struct sockaddr*)&serverAddr, addr_size);
  
   // wait for ack
   recv_ack(&buf, nbytes);
  
   return;
}
  
//***********************
// wait for ack
// **********************
void recv_ack (PACKET *buf, int nbytes) 
{
   HEADER  receive_buf;
   int     cs;
   int     r;
   struct timeval tv;
   int     rv;
   int     counter = 0;
  
   // The fd_set data type represents file descriptor sets for the select function
   // It is actually a bit array
   // (set of socket descriptors)
   fd_set readfds;
  
   fcntl(sock, F_SETFL, O_NONBLOCK);
  
   while (1) {
       while (1) {
           FD_ZERO (&readfds); // Clear an fd_set
          
           // void FD_SET (int filedes, fd_set *set)
           // This macro adds filedes to the file descriptor set set.
           FD_SET (sock, &readfds);
          
           tv.tv_sec = 5;
           tv.tv_usec = 0;
           
           // int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
           // maxfd: maximum number of descriptors ready.
           // timeout: How long to wait for select to return.
           //The select function blocks , till an activity occurs
          
           // On success, select() and pselect() return the number of file
           // descriptors contained in the three returned descriptor sets (that is,
           // the total number of bits that are set in readfds, writefds,
           // exceptfds) which may be zero if the timeout expires before anything
           // interesting happens
          
           // in the following we only pass readfds because we are interested in reading only
           rv = select (sock + 1, &readfds, NULL, NULL, &tv);
          
          
           // return value is 1 when there is a bit set in readfds
              if (rv == 1) {
                printf ("data is available now.\n");
                break;
              }
          
           // this means select() returned due to timeout event
           else if (rv == 0) {
               printf ("timeout\n"); 
                counter++; // increment counter
                if (counter == 3) { // if counter reaches 3, exit
                    printf ("Server is not responding. Exiting...\n");
                    exit(1);
                } 
                else { // else resend the packet
                    printf ("Resending packet...\n");  
                    sendto(sock, buf, sizeof(HEADER)+nbytes, 0, (struct sockaddr*)&serverAddr, addr_size); 
                }
           }

              // this means select() returned due to an error
              else {
                printf ("select error\n");
                exit(1);
              }
       }
      
       // we break the previous while(1) loop when there is data to be read (ack received)
      
       recvfrom (sock, &receive_buf, sizeof(HEADER), 0, NULL, NULL);
      
       cs = receive_buf.checksum;
       receive_buf.checksum = 0;
       
       // recalculate checksum of the received ack packet
       receive_buf.checksum = calc_checksum((char *)&buf, sizeof (HEADER) + nbytes);
      
       // a bad ack has been received
         if (cs != receive_buf.checksum) { // checksums do not match
              printf ("Bad ack received. Resending packet...\n");
              sendto(sock, buf, sizeof(HEADER)+nbytes, 0, (struct sockaddr*)&serverAddr, addr_size);
         }

         // a good ack has been received
            else {
                printf ("Good ack received. ");
                if (receive_buf.seq_ack == 0) {
                    printf ("Next packet expected is 1\n");
                    state = 1;
                }
                else {
                    printf ("Next packet expected is 0\n");
                    state = 0;
                }
                break;
            } 
      
       // resend packet
       // simulating erroneous channel...corruption and loss
       
         // the probability of correct checksum is 80%
            r = rand () % 10;
            if (r <= 8)
                buf->header.checksum = calc_checksum ((char *)buf, sizeof (HEADER) + nbytes);
            else
                printf ("Packet got corrupted on the way!\n");

            // the probability of no packet loss is 80%
            r = rand () % 10;

            if (r <= 8)
                sendto(sock, buf, sizeof(HEADER)+nbytes, 0, (struct sockaddr*)&serverAddr, addr_size);
            else
                printf ("Packet got lost on the way!\n");

   }
}
 
//***********************
// calculate checksum by using XOR
// **********************
int calc_checksum (char *buf, int tbytes) {
   int     i;
   char    cs = 0;
   char    *p;


   p = (char *)buf;
   cs = *p;
    for (i = 1; i < tbytes; i++) {
         cs = cs ^ *p;
         printf("calc_checksum20: i=%3d, cs=%3d, char=%c char_value=%d\n", i, cs, (char)*p, *p);
    }
  
   return (int)cs;
} 
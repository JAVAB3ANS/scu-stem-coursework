// SCU COEN 146
//
// This program implements the server side of stop & wait protocol
// the server receives file contents from a client
//
//
// For the stop and wait protocol, the assumptions are:
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


#include    "tfv1.h"


// global variables
int state = 0; // we only have two states: 0 and 1
int sock;
struct sockaddr_in serverAddr;
socklen_t addr_size;


char buffer[1024];




// list of functions
int main (int, char *[]);
int my_receive (PACKET *);
int calc_checksum (char *, int);




int main (int argc, char *argv[])
{
   FILE *fp;
   int n;
   PACKET buf;


   if (argc != 2)
   {
       printf ("need the port number\n");
       return 1;
   }


   memset((char *)serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(atoi(argv[1]));
   serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr_size = sizeof(serverAddr);


   // create socket
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
       printf ("socket error\n");
       return 1;
   }


   // bind
   if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0) {
       printf ("bind error\n");
       return 1;
   }


   // NOTE: this program uses UDP socket, so there is no need to listen to incoming connections!


   // receive name of the file
   // my_receive() function ensures the received data chunks are not corrupted
   if ((n = my_receive(&buf)) <= 0) {
       printf ("could not get the name of the file\n");
       return 1;
   }
   buf.data[5] = '\0';
   printf ("File name %s has been received!\n", buf.data);


   // open file
   if ((fp = fopen(buf.data, "wb")) == NULL) {
       printf ("error fopen\n");
       return 1;
   }


   printf ("Receiving file %s ... \n", buf.data);
   // my_receive() function ensures the received data chunks are not corrupted
   while ((n = my_receive (&buf)) > 0) {
       printf ("writing to file... n = %d\n", n);
           fwrite (buf.data, sizeof(char), n, fp);
       printf("------\n");
   }


   close (sock);
   fclose (fp);


   return 0;
}




   // my_receive() function ensures the received data chunks are not corrupted
   int my_receive (PACKET *recv_pck) {
   int cs_in;
   int cs_calc;
   int d;
   int r;
   int nbytes;


   HEADER          ack_packet;
   struct sockaddr ret_addr;
   socklen_t       addr_size = sizeof (ret_addr);




   while (1) {
       // ssize_t recvfrom(int socket, void *restrict buffer, size_t length,
       //          int flags, struct sockaddr *restrict address, socklen_t *restrict address_len);
       // buffer: Points to the buffer where the message should be stored.
       // address: A null pointer, or points to a sockaddr structure in which the sending address is to be stored.
       // The length and format of the address depend on the address family of the socket.
       // address_len: Specifies the length of the sockaddr structure pointed to by the address argument.


        // receive packet
        nbytes = recvfrom (sock, buffer, sizeof(buffer), 0, &ret_addr, &addr_size);
        if (nbytes < 0) {
            printf ("recvfrom error\n");
            return -1;
        }

       printf ("Received a UDP packet!\n");
    //    printf("%s\n", recv_pck->data);

    //    cs_in = recv_pck->header.checksum;
    //    recv_pck->header.checksum = 0;

    //    // recalculate checksum
    //    cs_calc = calc_checksum(buffer, nbytes);

    //    // check if checksum matches, and the sequence number is correct too
    //    //if (cs_in == cs_calc /*&& recv_pck->header.seq_ack == state*/)
    //    {
    //        printf ("Checksum passed! Sequence number matched!\n");


    //        // good packet
        //         // send ACK
        //         ack_packet.seq_ack = state;
        //         ack_packet.checksum = calc_checksum((char *)&ack_packet, sizeof(HEADER));
        //         //sendto (sock, &ack_packet, sizeof(HEADER), 0, &ret_addr, addr_size);
        //         printf ("Sent ACK for sequence number: %d...\n", ack_packet.seq_ack );


    //        // simulating erroneous channel...corruption and loss
    //        int r = rand() % 10; // random number between 0 to 9
    //        // everything ok case

    //        if (r <= 8) {
        //                 // change state
        //                 state = 1 - state;
        //                 return recv_pck->header.len;

    //        }
    //        // False case
    //        else{
        //                 printf ("Resending ack for sequence number: %d...\n", ack_packet.seq_ack );
        //                 sendto (sock, &ack_packet, sizeof(HEADER), 0, &ret_addr, addr_size);
        //                 printf ("Sent ACK for sequence number: %d...\n", ack_packet.seq_ack );

        //         }

        //         // now we have to wait for the next packet

        //         // change state
        //         state = 1 - state;
    //        printf("Length is : %d...\n", recv_pck->header.len);
                   return recv_pck->header.len;
       }
       /*else
       {
           printf ("Checksum/sequence number check failed!\n");


           // bad packet
                   // send ACK
                   ack_packet.seq_ack = 1 - state;
                   ack_packet.checksum = calc_checksum((char *)&ack_packet, sizeof(HEADER));
                   //sendto (sock, &ack_packet, sizeof(HEADER), 0, &ret_addr, addr_size);


           printf ("Resending ack for sequence number: %d...\n", ack_packet.seq_ack );


           // simulating erroneous channel...corruption and loss
           int r = rand() % 10; // random number between 0 to 9
           // everything ok case

           if (r <= 8){
                           // change state
                           state = 1 - state;
                           return recv_pck->header.len;
           }
           // False case
           else {
                           printf ("Resending ack for sequence number: %d...\n", ack_packet.seq_ack );
                           //sendto (sock, &ack_packet, sizeof(HEADER), 0, &ret_addr, addr_size);
                           printf ("Sent ACK for sequence number: %d...\n", ack_packet.seq_ack );
           }


       } */
   //}


   return -1;
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
         //printf("calc_checksum20: i=%3d, cs=%3d, char=%c char_value=%d\n", i, cs, (char)*p, *p);
    }

   return (int)cs;
}
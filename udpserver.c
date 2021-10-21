#include "udp.h"

int main(void) {

   int sock_server;  /* Socket on which server listens to clients */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   req_packet_t *req_message=(req_packet_t*)malloc(sizeof(req_packet_t)+1);  /* receive message */
   ret_packet_t *ret_message=(ret_packet_t*)malloc(sizeof(ret_packet_t)); /* return message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   bytes_recd=0;
   int total_bytes_sent=0;
   unsigned int i;  /* temporary loop variable */

   /* open a socket */

   if ((sock_server = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("Server: can't open datagram socket\n");
      exit(1);
   }

   /* initialize server address information */

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */
   server_port = SERV_UDP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address\n");
      close(sock_server);
      exit(1);
   }

   /* wait for incoming messages in an indefinite loop */

   printf("Waiting for incoming messages on port %hu\n\n", 
                           server_port);

   client_addr_len = sizeof (client_addr);

   for (;;) {
     while(bytes_recd==0){
       bytes_recd = recvfrom(sock_server, req_message, sizeof(req_message)+1, 0,
                     (struct sockaddr *) &client_addr, &client_addr_len);
     }
       printf("Before convert req_message\n");
      convertReq(req_message, 0);
      printf("Received Sentence is: %s\n     with length %d\n\n",
                         "message", bytes_recd);

      /* prepare the message to send */

      msg_len = bytes_recd;
      bytes_recd=0;
      //For each packet except the final, make a return packet and send it out. Increment bytes
      for (i=1; i<req_message->count/25; i++) {
	      makeRetPacket(ret_message,req_message->req_id,i,0,25);
         bytes_sent = sendto(sock_server, ret_message, sizeof(ret_packet_t), 0,
               (struct sockaddr*) &client_addr, client_addr_len);
         total_bytes_sent=total_bytes_sent+bytes_sent;
      }
      //If last packet is not full payload, send final full payload and then final packet with lesser payload
      if(req_message->count%25!=0){
	      makeRetPacket(ret_message,req_message->req_id,i,0,25);
         bytes_sent = sendto(sock_server, ret_message, sizeof(ret_packet_t), 0,
               (struct sockaddr*) &client_addr, client_addr_len);
         total_bytes_sent=total_bytes_sent+bytes_sent;
	      makeRetPacket(ret_message,req_message->req_id,i,1,req_message->count%25);
	      bytes_sent = sendto(sock_server, ret_message, 8+4*(req_message->count%25), 0,
               (struct sockaddr*) &client_addr, client_addr_len);
	      total_bytes_sent=total_bytes_sent+bytes_sent;
      }
      //Else send last packet since it has full payload.
      else{
	      makeRetPacket(ret_message,req_message->req_id,i,1,25);
         bytes_sent = sendto(sock_server, ret_message, sizeof(ret_packet_t), 0,
               (struct sockaddr*) &client_addr, client_addr_len);
         total_bytes_sent=total_bytes_sent+bytes_sent;
      }
      printf("Total Bytes Sent: %d\n",total_bytes_sent);
   }
 }
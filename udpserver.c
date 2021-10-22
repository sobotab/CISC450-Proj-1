#include "udp.h"


int main(void) {
   
   time_t t;
   srand((unsigned) time(&t));
   
   int sock_server;  /* Socket on which server listens to clients */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   req_packet_t *req_message=(req_packet_t*)malloc(sizeof(req_packet_t)+1);  /* receive message */
   ret_packet_t **ret_message; /* return message */
   int bytes_sent=0, total_bytes_sent=0, total_bytes_recd=0, bytes_recd=0; /* number of bytes sent or received */
   int total_packets=0;
   int packet_count;
   unsigned short int seq_sum=0;
   unsigned int check_sum=0;

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
      printf("\nWaiting for incoming message...\n");
      bytes_recd = recvfrom(sock_server, req_message, sizeof(req_message)+1, 0,
                     (struct sockaddr *) &client_addr, &client_addr_len);
      convertReq(req_message, 0);
      if(req_message->count%25==0) {
	      packet_count=req_message->count/25;
      } else {
	      packet_count=req_message->count/25+1;
      }

      /* prepare the message to send */
      ret_packet_t ret_message[packet_count];
      makeRetMessage(ret_message, req_message->req_id, req_message->count);
	
      /*printf("ret_message: \n");
      for(int i=0; i<req_message->count/25+1; i++) {
	      printf("req_id: %hu, seq_num: %hu, last: %hu, count: %hu payload: \n", ret_message[i].req_id, ret_message[i].seq_num, ret_message[i].last, ret_message[i].count);
	      for (int j=0; j<25; j++) {
		      printf("\tindex: %d, value: %ld\n", j, ret_message[i].payload[j]);
	      }
      }*/

      /* send message */
      for(int i=0; i<packet_count; i++) {
	      seq_sum+=ret_message[i].seq_num;
	      for(int j=0; j<25; j++) {
		      check_sum+=ret_message[i].payload[j];
	      }
      }
      convertRet(ret_message, packet_count, 1);
	
      /*for(int i=0; i<req_message->count/25+1; i++) {
              printf("req_id: %hu, seq_num: %hu, last: %hu, count: %hu payload: \n", ret_message[i].req_id, ret_message[i].seq_num, ret_message[i].last, ret_message[i].count);
              for (int j=0; j<25; j++) {
                      printf("\tindex: %d, value: %ld\n", j, ret_message[i].payload[j]);
              }
      }*/

      bytes_sent = sendto(sock_server, ret_message, sizeof(ret_packet_t)*packet_count, 0,
               (struct sockaddr*) &client_addr, client_addr_len);

      /*Print info from packets received/sent*/
      total_packets+=packet_count;
      total_bytes_sent+=bytes_sent;
      printf("Message received: ");
      printf("Request ID: %hu\tCount: %hu\n", req_message->req_id, req_message->count);
      printf("\tNo. of Response Packets:\n\tThis Time: %d\tTotal: %d\n",packet_count, total_packets);
      printf("\tNo. of Bytes Transmitted:\n\tThis Time: %d\tTotal: %d\n",bytes_sent, total_bytes_sent);
      printf("\tSum of All Packet Sequence Numbers: %hu\n", seq_sum);
      seq_sum=0;
      printf("\tChecksum of Entire Payload: %d\n", check_sum);
      check_sum=0;
   }
 }

#include "udp.h"

int main(void) {

   int sock_client;  /* Socket used by client */ 

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned short client_port;  /* Port number used by client (local port) */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   short unsigned count;  /* send message */
   ret_packet_t **ret_message; /* receive message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
  
   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("Client: can't open datagram socket\n");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information
            unless you want to specify a specific local port.
            The local address initialization and binding is done automatically
            when the sendto function is called later, if the socket has not
            already been bound. 
            The code below illustrates how to initialize and bind to a
            specific local port, if that is desired. */

   /* initialize client address information */

   client_port = 0;   /* This allows choice of any available local port */

   /* Uncomment the lines below if you want to specify a particular 
             local port: */
   /*
   printf("Enter port number for client: ");
   scanf("%hu", &client_port);
   */

   /* clear client address structure and initialize with client address */
   memset(&client_addr, 0, sizeof(client_addr));
   client_addr.sin_family = AF_INET;
   client_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* This allows choice of
                                        any host interface, if more than one 
                                        are present */
   client_addr.sin_port = htons(client_port);

   /* bind the socket to the local client port */

   if (bind(sock_client, (struct sockaddr *) &client_addr,
                                    sizeof (client_addr)) < 0) {
      perror("Client: can't bind to local address\n");
      close(sock_client);
      exit(1);
   }

   /* end of local address initialization and binding */

   /* initialize server address information */

   printf("Enter hostname of server: ");
   scanf("%s", server_hostname);
   if ((server_hp = gethostbyname(server_hostname)) == NULL) {
      perror("Client: invalid server hostname\n");
      close(sock_client);
      exit(1);
   }
   printf("Enter port number for server: ");
   scanf("%hu", &server_port);

   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
   server_addr.sin_port = htons(server_port);

   /* user interface */
   for(;;) {
   	printf("Please input a sentence:\n");
   	scanf("%hu", &count);

   	/* send message */
  	req_packet_t *req_packet=malloc(sizeof(req_packet));
	req_packet=makeReqPacket(1,count);
	convertReq(req_packet, 1);
   	bytes_sent = sendto(sock_client, req_packet, sizeof(req_packet_t), 0,
         	   (struct sockaddr *) &server_addr, sizeof (server_addr));
   	/* get response from server */
	msg_len=sizeof(ret_packet_t) * count;
	
	ret_message=(ret_packet_t **)malloc(sizeof(ret_packet_t)*(count/25+1)+1);
	for (int i=0; i<(count/25+1); i++) {
		printf("making space for ret_message: %d\n", i);
		ret_message[i]=(ret_packet_t *)malloc(sizeof(ret_packet_t)+1);
	}
  
   	printf("Waiting for response from server...\n");
   	bytes_recd = recvfrom(sock_client, ret_message, msg_len, 0,
        	        (struct sockaddr *) 0, (int *) 0);
	convertRet(ret_message, (int)(count/25+1), 0);
   	printf("Made it here?\n");
	printf("\nThe response from server is: %hu\n", ret_message[0]->count);
	free(ret_message);
   }
   /* close the socket */

   close (sock_client);
}

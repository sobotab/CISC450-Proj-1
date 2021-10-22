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
   ret_packet_t *ret_message; /* receive message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, total_bytes_sent=0, total_bytes_recd=0, bytes_recd; /* number of bytes sent or received */
   unsigned short int i=1;
   int keepGoing=1;
   int correctInput=1;
   int packet_count=0;
   int total_packets=0;
   unsigned short int seq_sum=0;
   unsigned int check_sum=0;
   char input[STRING_SIZE];

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
   for(;keepGoing;) {
   	printf("Please input size of payload:\n");
   	while(correctInput) {
		scanf("%s", input);
		if (count<65535 && count>1 && !isdigit(input)) {
			correctInput=0;
			count=atoi(input);
		}else {
			printf("Make sure your input is a number between 1-65535\n");
		}
	}
	correctInput=0;
	if (count%25==0) {
		packet_count=count/25;
	} else {
		packet_count=count/25+1;
	}
   	/* send message */
  	req_packet_t *req_packet=malloc(sizeof(req_packet));
	req_packet=makeReqPacket(i,count);
	convertReq(req_packet, 1);
   	bytes_sent = sendto(sock_client, req_packet, sizeof(req_packet_t), 0,
         	   (struct sockaddr *) &server_addr, sizeof (server_addr));
   	
	
	/* get response from server*/
	ret_message=(ret_packet_t *)malloc(sizeof(ret_packet_t)*packet_count);
   	printf("Waiting for response from server...\n");
   	bytes_recd = recvfrom(sock_client, ret_message, sizeof(ret_packet_t)*packet_count, 0,
       		(struct sockaddr *) 0, (int *) 0);
	
	
	/*for(int i=0; i<count/25+1; i++) {
              printf("req_id: %hu, seq_num: %hu, last: %hu, count: %hu payload: \n", ret_message[i].req_id, ret_message[i].seq_num, ret_message[i].last, ret_message[i].count);
              for (int j=0; j<25; j++) {
                      printf("\tindex: %d, value: %ld\n", j, ret_message[i].payload[j]);
              }
        }*/

	convertRet(ret_message, packet_count, 0);
	
	/*for(int i=0; i<count/25+1; i++) {
              printf("req_id: %hu, seq_num: %hu, last: %hu, count: %hu payload: \n", ret_message[i].req_id, ret_message[i].seq_num, ret_message[i].last, ret_message[i].count);
              for (int j=0; j<25; j++) {
                      printf("\tindex: %d, value: %ld\n", j, ret_message[i].payload[j]);
              }
        }*/
	/*Print info*/
	total_packets+=packet_count;
	total_bytes_recd+=bytes_recd;
	for(int i=0; i<packet_count; i++) {
              seq_sum+=ret_message[i].seq_num;
              for(int j=0; j<25; j++) {
                      check_sum+=ret_message[i].payload[j];
              }
        }

	printf("\nRequest ID: %hu\tCount: %hu\n", i, count);
	printf("\tNo. of Response Packets:\n\tThis Time: %d\tTotal: %d\n",packet_count, total_packets);
	printf("\tNo. of Bytes Received:\n\tThis Time: %d\tTotal: %d\n",bytes_recd, total_bytes_recd);
        printf("\tSum of All Packet Sequence Numbers: %hu\n", seq_sum);
        seq_sum=0;
        printf("\tChecksum of Entire Payload: %d\n\n", check_sum);
        check_sum=0;


	free(ret_message);
	i++;

	printf("Type 1 and send another packet OR type 0 and enter to exit: \n");
	do {
                scanf("%d", &keepGoing);
                if (count<65535 && count>1 && !isdigit(count)) {
                        correctInput=1;
                }else {
                        printf("Make sure your input is 1 or 0\n");
                }
        } while(!correctInput);

   }
   /* close the socket */

   close (sock_client);
}

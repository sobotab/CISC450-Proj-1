//Included all packages from base files given by prof

#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, sendto, and recvfrom */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */
#include <time.h>
#include <netdb.h>	    /* for struct hostent and gethostbyname */

#define STRING_SIZE 1024

/* SERV_UDP_PORT is the port number on which the server listens for
   incoming messages from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_UDP_PORT 65129

typedef struct req_packet {
	//Request packet, requested by client
	unsigned short req_id;
	unsigned short count;
} req_packet_t;

typedef struct ret_packet {
	//Returned packet, returned by server
	unsigned short req_id;
	unsigned short seq_num;
	unsigned short last;
	unsigned short count;
	unsigned int payload[25];
} ret_packet_t;

void convertReq(req_packet_t *request_packet, int network);
void convertRet(ret_packet_t **return_packet, int length, int network);
void convertRetNonNetwork(ret_packet_t *return_packet, int length, int network);

//Used by udpserver
void makeRetPacket(ret_packet_t *new_packet, unsigned short req_id, 
		unsigned short seq_num, unsigned short last, unsigned short count);
void makeRetMessage(ret_packet_t** return_message, unsigned short req_id, unsigned short count);

//Used by udpclient
req_packet_t *makeReqPacket(unsigned short req_id, unsigned short count);

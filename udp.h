#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, sendto, and recvfrom */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#include <netdb.h>	    /* for struct hostent and gethostbyname */

#define STRING_SIZE 1024

/* SERV_UDP_PORT is the port number on which the server listens for
   incoming messages from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_UDP_PORT 65129

typedef struct req_packet {
	unsigned short req_id;
	unsigned short count;
} req_packet_t;

typedef struct ret_packet {
	unsigned short req_id;
	unsigned short seq_num;
	unsigned short last;
	unsigned short count;
	unsigned int payload[25];
} ret_packet_t;

ret_packet_t *makeRetPacket(unsigned short req_id, unsigned short seq_num, unsigned short last, unsigned short count);
ret_packet_t **makeRetMessage(unsigned short req_id, unsigned short count);
req_packet_t *makeReqPacket(unsigned short req_id, unsigned short count);

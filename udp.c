#include "udp.h"

ret_packet_t *makeRetPacket(unsigned short req_id, unsigned short seq_num, unsigned short last, unsigned short count) {
	printf("Beginning makeRetPacket...\n");
	ret_packet_t *new_packet= malloc(sizeof(ret_packet_t));
	new_packet->req_id=req_id;
	new_packet->seq_num=seq_num;
	new_packet->last=last;
	for (int i=0; i<count; i++) {
		new_packet->payload[i]=rand()%65536;
	}
	printf("Leaving makeRetPacket...\n");
	return new_packet;
}

ret_packet_t **makeRetMessage(unsigned short req_id, unsigned short count) {
	printf("Beginning makeRetMessage...\n");
	short remaining_count=count;
	short count_arg=25;
	int last=0;
	int i=1;
	ret_packet_t **new_message=malloc(sizeof(ret_packet_t)*(count/25+1));
	while (!last && remaining_count) {
		if(remaining_count > 25) {
			remaining_count=remaining_count-count_arg;
		} else {
			last = 1;
			count_arg=remaining_count;
		}
		makeRetPacket(req_id, i++, last, count_arg);
	}
	printf("Leaving makeRetMessage...\n");
	return new_message;
}

req_packet_t *makeReqPacket(unsigned short req_id, unsigned short count) {
	req_packet_t *new_packet=malloc(sizeof(req_packet_t));
	new_packet->req_id=req_id;
	new_packet->count=count;
	return new_packet;
}

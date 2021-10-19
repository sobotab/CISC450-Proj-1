#include "udp.h"

ret_packet_t *makePacket(unsigned short req_id, unsigned short seq_num, unsigned short last, unsigned short count) {
	*ret_packet_t new_packet= malloc(sizeof(ret_packet_t));
	new_packet->req_id=req_id;
	new_packet->seq_num=seq_num;
	new_packet->last=last;
	for (int i=0; i<count; i++) {
		new_packet->payload[i]=rand()%65536;
	}
	return new_packet;
}

ret_packet_t **makeMessage(unsigned short req_id, unsigned short count) {
	short remaining_count=count;
	short count_arg=25;
	int last=0;
	i=1;
	ret_packet_t **new_message=malloc(sizeof(ret_packet)*(count/25+1));
	while (!last && remaining_count) {
		if(remaining_count > 25) {
			remaining_count=remaining_count-count_arg;
		} else {
			last = 1;
			count_arg=remaining_count;
		}
		makePacket(req_id, i++, last, count_arg);
	}
	return new_message;
}

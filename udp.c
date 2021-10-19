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

ret_packet_t **makeMessage(unsigned int req_id, unsigned int count) {
	ret_packet_t **new_message=malloc(sizeof(ret_packet)*(count/25+1));
	while (count) {
		makePacket(req_id, i, last_bool, count); //fix this and create values

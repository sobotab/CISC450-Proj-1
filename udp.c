#include "udp.h"

void makeRetPacket(ret_packet_t *new_packet, unsigned short req_id, 
		unsigned short seq_num, unsigned short last, unsigned short count) {
	/* Desc: assigns values to the struct and returns it. Can have a maximum payload of 25
	 *
	 */
	printf("Beginning makeRetPacket...\n");
	new_packet->req_id=req_id;
	new_packet->seq_num=seq_num;
	new_packet->last=last;
	new_packet->count=count;
	for (int i=0; i<count; i++) {
		new_packet->payload[i]=rand()%65536;
	}
	printf("Leaving makeRetPacket...\n");
}

void makeRetMessage(ret_packet_t** return_message, unsigned short req_id, unsigned short count) {
	/* Creates an array of packets using makeRetPacket
	 *
	 */
	printf("Beginning makeRetMessage...\n");
	short remaining_count=count;
	short count_arg=25;
	int last=0;
	int i=0;
	while (!last && remaining_count) {
		if(remaining_count > 25) {
			printf("making packet of size 25\n");
			remaining_count=remaining_count-count_arg;
		} else {
			printf("making packet of size <25\n");
			last = 1;
			count_arg=remaining_count;
		}
		makeRetPacket(return_message[i], req_id, i, last, count_arg);
		printf("packet %d count: %hu\n", i, return_message[i]->count);
		i++;
	}
	printf("Leaving makeRetMessage...\n");
}

req_packet_t *makeReqPacket(unsigned short req_id, unsigned short count) {
	/*
	 * Assigns values to req_packet_t
	 */
	printf("Beginnging makeReqPacket...\n");
	req_packet_t *new_packet=malloc(sizeof(req_packet_t));
	new_packet->req_id=req_id;
	new_packet->count=count;
	printf("Leaving makeReqPacket...\n");
	return new_packet;
}

void convertReq(req_packet_t *request_packet, int network) {
	/* Values need to be transmitted to network values so they can be sent
	 * Network int is used as bool for whether packet should be converted to network values
	 */
	printf("Beginning convertReq...\n");
	if (network) {
		request_packet->req_id=htons(request_packet->req_id);
		request_packet->count=htons(request_packet->count);
	} else {
		request_packet->req_id=ntohs(request_packet->req_id);
		request_packet->count=ntohs(request_packet->count);
	}
	printf("Leaving convertReq...\n");
}

void convertRet(ret_packet_t **return_packet, int length, int network) {
	/* Values need to be converted to network values so they can be sent
	 * Network int is a bool
	 */
	printf("Beginning convertRet...\n");
	if (network) {
		for (int i=0; i < length; i++) {
			return_packet[i]->req_id=htons(return_packet[i]->req_id);
			return_packet[i]->seq_num=htons(return_packet[i]->seq_num);
			return_packet[i]->last=htons(return_packet[i]->last);
			return_packet[i]->count=htons(return_packet[i]->count);
			for(int j=0; return_packet[i]->payload[j] && j<25; j++) {
				return_packet[i]->payload[j]=htonl(return_packet[i]->payload[j]);
			}
		}
	} else {
		for (int i=0; i < length; i++) {
			printf("convertRet loop packet: %d\n", i);
                        return_packet[i]->req_id=ntohs(return_packet[i]->req_id);
                        return_packet[i]->seq_num=ntohs(return_packet[i]->seq_num);
                        return_packet[i]->last=ntohs(return_packet[i]->last);
                        return_packet[i]->count=ntohs(return_packet[i]->count);
                        for(int j=0; return_packet[i]->payload[j] && j<25; j++) {
                                printf("convertRet loop payload: %d\n", j);
				return_packet[i]->payload[j]=ntohl(return_packet[i]->payload[j]);
                        }
                }
	}
	printf("Leaving convertRet...\n");
}

void convertRetNonNetwork(ret_packet_t *return_packet, int length, int network) {
	for (int i=0; i < length; i++) {
	        printf("convertRet loop packet: %d\n", i);
                return_packet[i].req_id=ntohs(return_packet[i].req_id);
                return_packet[i].seq_num=ntohs(return_packet[i].seq_num);
                return_packet[i].last=ntohs(return_packet[i].last);
                return_packet[i].count=ntohs(return_packet[i].count);
                for(int j=0; j<25; j++) {
                       printf("convertRet loop payload: %d\n", j);
		       printf("return packet[%d].payload[%d]=%d",i,j,return_packet[i].payload[j]);
                       return_packet[i].payload[j]=ntohl(return_packet[i].payload[j]);
                }
        }
}

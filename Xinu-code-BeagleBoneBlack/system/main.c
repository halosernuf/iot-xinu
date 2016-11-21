/*  main.c  - main */

#include <xinu.h>
#define EDGE_PORT 500001
#define LOCAL_PORT 500001

struct udp_boot_msg{
	char msg[5]="boot/";
	uint32 veri;
};
struct ack_boot_msg{
	uint32 ack_veri;
	uint32 id;
};

uint32 write_boot_msg(struct udp_boot_msg* boot_msg){
	
	getutime(boot_msg->veri);
	return (uint32)(sizeof(struct udp_boot_msg));
}
syscall udp_bootstrap(uint32 *edgeip){
	//boardcast init of platfrom
	struct udp_boot_msg boot_msg;
	struct ack_boot_msg ack_msg;
	uint32 len = write_boot_msg(&boot_msg);
	uint32 inlen;
	uint32 remip;
	uint16 remport;
	//boardcast bootstrap message
	slot = udp_register(0, EDGE_PORT, LOCAL_PORT);
	udp_sendto(slot, IP_BCAST, EDGE_PORT,
		(char *)&boot_msg, len);
	
	for (j=0; j<3; j++) {
		inlen = udp_recvaddr (slot, &remip, &remport, (char*)&ack_msg,sizeof(struct ack_msg),2000);
		if (inlen == TIMEOUT) {
			continue;
		} else if (inlen == SYSERR) {
			return SYSERR;
		}
		
	}
}
process udp_routine(){
	uint32 edgeip;
	udp_bootstrap(&edgeip);
}

process	main(void)
{
	
	return OK;
    
}

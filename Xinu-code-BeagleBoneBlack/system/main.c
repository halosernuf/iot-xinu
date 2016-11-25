/*  main.c  - main */

#include <xinu.h>
// #include <delay.h>
#include <string.h>
// #define GPIO_1 0x4804C000
// #define MAXTIMINGS	85
// #define DHTPIN 7
#define DEV_MAXNAM	20
#define EDGE_PORT	50001
#define LOCAL_PORT	50001
// struct udp_boot_msg{
// 	char msg[5]="boot/";
// 	uint32 veri;
// };
// struct ack_boot_msg{
// 	uint32 ack_veri;
// 	uint32 id;
// };

// uint32 write_boot_msg(struct udp_boot_msg* boot_msg){
	
// 	getutime(boot_msg->veri);
// 	return (uint32)(sizeof(struct udp_boot_msg));
// }
// syscall udp_bootstrap(uint32 *edgeip){
// 	//boardcast init of platfrom
// 	struct udp_boot_msg boot_msg;
// 	struct ack_boot_msg ack_msg;
// 	uint32 len = write_boot_msg(&boot_msg);
// 	uint32 inlen;
// 	uint32 remip;
// 	uint16 remport;
// 	//boardcast bootstrap message
// 	slot = udp_register(0, EDGE_PORT, LOCAL_PORT);
// 	udp_sendto(slot, IP_BCAST, EDGE_PORT,
// 		(char *)&boot_msg, len);
	
// 	for (j=0; j<3; j++) {
// 		inlen = udp_recvaddr (slot, &remip, &remport, (char*)&ack_msg,sizeof(struct ack_msg),2000);
// 		if (inlen == TIMEOUT) {
// 			continue;
// 		} else if (inlen == SYSERR) {
// 			return SYSERR;
// 		}
		
// 	}
// }
// process udp_routine(){
// 	uint32 edgeip;
// 	udp_bootstrap(&edgeip);
// }

// #pragma pack(1)
// struct	device_msg {
// 	union {
// 	 /* Items in a device_name */
// 	 struct	{
// 	  char	device_name[DEV_MAXNAM];  file name and mode	
// 	 };
// 	 /* Items in a device_id */
// 	 struct {
// 	  char	device_data[TFTP_MAXDATA]; /* Actual data		*/
// 	 };
// 	};
// };
// #pragma pack()

// syscall make_msg(char* ori_str,int ){
	
// }
process udp_routine(){
	uint32 slot;
	char remoteipdot[]="192.168.1.21";
	char msg_rvc[20];
	memset((char *)&msg_rvc, '\0', sizeof(msg_rvc));
	// char msg_snd[]
	did32 i;
	char ret[20];
	char msg[20];
	char end_msg[3]="end";
	uint32 remoteip;
	// uint16 remoteport=50001;
	// uint16 localport=50001;
	uint32 inlen;
	uint32 ipaddr=NetData.ipucast;
	dot2ip((char *)&remoteipdot,&remoteip);
	printf("udp start\n");
	printf("%x",remoteip);
	slot = udp_register(0, EDGE_PORT, LOCAL_PORT);
	// slot2 = 
	// udp_send(slot,msg_snd,strlen((char *)&msg_snd));
	while(1){
		// if(udp_sendto(slot,0xC0A80115,EDGE_PORT,(char*)&msg_snd,strlen((char *)&msg_snd))!=OK){
		// 	printf("send error\n");
		// }
		// printf("sned msg_snd %s \n",msg_snd);
		// sleep(10);
		inlen = udp_recv(slot, (char *)&msg_rvc,
				    20, 2000);
		if (inlen == TIMEOUT) {
			printf("TIMEOUT\n");
			continue;
		} else if (inlen == SYSERR) {
			printf("SYSERR\n");
			continue;
		}
		printf("%s\n", msg_rvc);
		if(strcmp(msg_rvc,"IP")==0){
			sprintf(msg, "%d.%d.%d.%d",
			(ipaddr>>24)&0xff, (ipaddr>>16)&0xff,
			(ipaddr>>8)&0xff,ipaddr&0xff);
			if(udp_sendto(slot,0xC0A80115,EDGE_PORT,(char*)&msg,sizeof(msg))!=OK){
				printf("send error\n %s",msg);
			}
			printf("%s \n", msg);
			// memset((char *)&ret, 0x00, sizeof(ret));
			memset((char *)&msg, '\0', sizeof(msg));
		}else if(strcmp(msg_rvc,"DEVICES")==0){
			for(i=26;i<NDEVS;i++){
				printf("read device %s\n",devtab[i].dvname);
				read(i,(char *)&ret,20);
				sprintf(msg,"%s %d %s",devtab[i].dvname,i,ret);
				printf("%s \n", msg);
				if(udp_sendto(slot,0xC0A80115,EDGE_PORT,(char*)&msg,sizeof(msg))!=OK){
					printf("send error\n %s",msg);
				}
				memset((char *)&ret, '\0', sizeof(ret));
				memset((char *)&msg, '\0', sizeof(msg));
			}
			//send ending
			if(udp_sendto(slot,0xC0A80115,EDGE_PORT,(char*)&end_msg,sizeof(end_msg))!=OK){
				printf("send error %s \n",msg);
			}
		}else{
			printf("msg error\n");
		}
		//clear msg_rvc
		memset((char *)&msg_rvc, '\0', sizeof(msg_rvc));
	}
}

process	main(void)
{
	
	resume(create(udp_routine, 4096, 50, "udp_routine", 0));
	return OK;
    
}

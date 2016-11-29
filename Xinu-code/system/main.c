/*  main.c  - main */

#include <xinu.h>
#include <string.h>
#define DEV_MAXNAM	20
#define EDGE_PORT	50001
#define LOCAL_PORT	50001

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
	uint32 remip;
	uint32 remport;
	uint32 inlen;
	uint32 ipaddr=NetData.ipucast;
	// dot2ip((char *)&remoteipdot,&remoteip);
	printf("udp start\n");
	// printf("%x",remoteip);
	slot = udp_register(0, EDGE_PORT, LOCAL_PORT);
	while(1){
		inlen = udp_recvaddr(slot, &remip, &remport,(char *)&msg_rvc,
				    20, 2000);
		printf("%x\n", remip);
		if (inlen == TIMEOUT) {
			// printf("TIMEOUT\n");
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
			if(udp_sendto(slot,remip,EDGE_PORT,(char*)&msg,sizeof(msg))!=OK){
				printf("send error\n %s",msg);
			}
			printf("%s \n", msg);
			memset((char *)&msg, '\0', sizeof(msg));
		}else if(strcmp(msg_rvc,"DEVICES")==0){
			for(i=26;i<NDEVS;i++){
				printf("read device %s\n",devtab[i].dvname);
				read(i,(char *)&ret,20);
				sprintf(msg,"%s %d %s",devtab[i].dvname,i,ret);
				printf("%s \n", msg);
				if(udp_sendto(slot,remip,EDGE_PORT,(char*)&msg,sizeof(msg))!=OK){
					printf("send error\n %s",msg);
				}
				memset((char *)&ret, '\0', sizeof(ret));
				memset((char *)&msg, '\0', sizeof(msg));
			}
			//send ending
			if(udp_sendto(slot,remip,EDGE_PORT,(char*)&end_msg,sizeof(end_msg))!=OK){
				printf("send error %s \n",msg);
			}
		}else if(strncmp(msg_rvc,"LED",3)==0){
    		char* ptr=msg_rvc+4;
    		uint32 dev_id=0;
    		while(*ptr!=' '){
        		dev_id=dev_id*10+*ptr-'0';
        		ptr++;
    		}
    		ptr++;
    		uint32 data=0;
    		while(*ptr!='\0'){
        		data=data*10+*ptr-'0';
        		ptr++;
    		}
			printf("write led %d %d\n", dev_id, data);
			write(dev_id,(char* )&data,1);
			if(udp_sendto(slot,remip,EDGE_PORT,(char*)&end_msg,sizeof(end_msg))!=OK){
				printf("send error %s \n",msg);
			}
		}else{
			printf("msg error\n");
		}
		//clear msg_rvc
		memset((char *)&msg_rvc, '\0', sizeof(msg_rvc));
	}
}

process dht11_routine(){
	uint32 dht11tmpthrd=20;
	uint32 dht11hudthrd=20;
	while(1){
		char ret[20];
		int32 dht11[4]={0,0,0,0};
		read(DHT11_0,(char *)&ret,20);
		printf("[local] dht11 %s\n", ret);
		char* ptr=ret;
		uint32 i=0;
		while(i<4 && *ptr!='\0'){
			while(*ptr!=' '){
				dht11[i]=dht11[i]*10+*ptr-'0';
				ptr++;
			}
			ptr++;
			i++;
		}
		printf("%d \n", dht11[0]);
		printf("%d \n", dht11[2]);
		uint32 one=1;
		uint32 zero=0;
		if(dht11[0]>dht11hudthrd){
			write(LED_1,(char *)&one,1);
		}else{
			write(LED_1,(char *)&zero,1);
		}
		if(dht11[2]>dht11tmpthrd){
			write(LED_2,(char *)&one,1);
		}else{
			write(LED_2,(char *)&zero,1);
		}
		sleep(5);
	}
}
process tmp36_routine(){
	uint32 tmp36thrd=20;
	while(1){
		char ret[20];
		read(TMP36_0,(char *)&ret,20);
		printf("[local] tmp36 %s\n", ret);
		char* ptr=ret;
		uint32 tmp=0;
		while(*ptr!='\0'){
			tmp=tmp*10+*ptr-'0';
			ptr++;
		}
		uint32 one=1;
		uint32 zero=0;
		if(tmp>tmp36thrd){
			write(LED_0,(char *)&one,1);
		}else{
			write(LED_0,(char *)&zero,1);
		}
		sleep(5);
	}
}

process	main(void)
{

	resume(create(udp_routine, 4096, 50, "udp_routine", 0));
	resume(create(tmp36_routine, 4096, 50, "tmp36_routine", 0));
	resume(create(dht11_routine, 4096, 50, "dht11_routine", 0));
	return OK;
    
}

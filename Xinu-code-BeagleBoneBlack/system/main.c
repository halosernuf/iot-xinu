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
	uint32 remoteip;
	uint32 inlen;
	uint32 ipaddr=NetData.ipucast;
	dot2ip((char *)&remoteipdot,&remoteip);
	printf("udp start\n");
	printf("%x",remoteip);
	slot = udp_register(0, EDGE_PORT, LOCAL_PORT);
	while(1){
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
	// uint32 data=1;
	// char read;
	// struct dentry	*devptr;
	// devptr = (struct dentry *) &devtab[29];
	
	// write(28,(char *)data, 1);
	// ledread(devptr,&read,1);
	// printf("%c\n",read);
	
	// ledwrite(devptr,&data,1);
	// ledread(devptr,&read,1);
	// printf("%c\n",read);
	// ledread(devptr,&read,1);
	// printf("%c\n",read);
	// ledwrite(devptr,&data,1);
	// devptr = (struct dentry *) &devtab[28];
	// ledwrite(devptr,&data,1);
	// ledread(devptr,&read,1);
	// printf("%c\n",read);
	// writeled(devptr,&data);
	
	// ledread(devptr,&read,1);
	// printf("%c\n",read);
	
	// uint32 GPIO=0x4804C000;
	// uint32 PINNUM=22;
	// struct	gpio1_csreg *gpio1ptr;
	// gpio1ptr = (struct gpio1_csreg *)GPIO;
	// uint32 pin=1<<(PINNUM%32);
	// //set pin to output
	// gpio1ptr->oe &= ~pin;
	// if(data){
	// 	gpio1ptr->dataout |= pin;
	// }else{
	// 	gpio1ptr->dataout &= ~pin;
	// }
	// while(1){};
	resume(create(udp_routine, 4096, 50, "udp_routine", 0));
	return OK;
    
}

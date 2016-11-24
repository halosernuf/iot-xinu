#include <xinu.h>
#include <delay.h>
#define MAXTIMINGS	85

syscall DHT11_read(
	struct dentry	*devptr,
	uint32 *data)
{
	uint32 dht11_dat[5] = { 0, 0, 0, 0, 0 };
	uint32 laststate	= 1;
	uint32 counter		= 0;
	uint32 j			= 0;
	uint32 i;
	uint32 GPIO;
	uint32 DHTPIN;
	uint32 PINNUM = (uint32)devptr->dvcsr;
	switch(PINNUM/32){
		case 0:
			GPIO=GPIO_0;
			break;
		case 1:
			GPIO=GPIO_1;
			break;
		case 2:
			GPIO=GPIO_2;
			break;
		case 3:
			GPIO=GPIO_3;
			break;
		default:
			return SYSERR;
	}
	DHTPIN=PINNUM%32;
	struct	gpio1_csreg *gpio1ptr;
	gpio1ptr = (struct gpio1_csreg *)GPIO;
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
	uint32 pin=1<<DHTPIN;
	gpio1ptr->oe &= ~pin;
	gpio1ptr->dataout &= ~pin; 
	MDELAY(18);//delay 18 millisecond
	gpio1ptr->dataout |= pin;
	DELAY(40);//delay 40 microsecond
	//set pin to input
	gpio1ptr->oe |= pin;
	/* detect change and read data */
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ((gpio1ptr->datain>>DHTPIN & 1) == laststate )
		{
			counter++;
			if ( counter == 1000 )
			{
				break;
			}
		}
		// printf("%x \n", gpio1ptr->datain>>6 &1);
		laststate = (gpio1ptr->datain>>DHTPIN & 1);
		if ( counter == 1000 )
			break;
		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			dht11_dat[j / 8] <<= 1;
			// cnt[j]=counter;
			if ( counter > 200 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if((j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
	{
		for(i=0;i<4;i++){
			data[i]=dht11_dat[i];
		}
	}else{
		printf("check CRC error\n");
		return SYSERR;
	}
	return OK;
}
// syscall DHT11_read(
// 	struct dentry	*devptr,
// 	uint32 *data)
// {
// 	uint32 dht11_dat[5] = { 0, 0, 0, 0, 0 };
// 	bool8 laststate		= 1;
// 	uint32 counter		= 0;
// 	uint32 j			= 0;
// 	uint32 cntTrhd		= 1000;
// 	uint32 ioTrhd		= 100;
// 	uint32 i;
// 	uint32 dhtpin;
// 	// dhtptr = &dhttab[devptr->dvminor];
// 	dhtpin = (uint32)devptr->dvcsr;
// 	printf("%d \n", dhtpin);
// 	MDELAY(18);
// 	gpio_write(dhtpin,1);
// 	// if(gpio_write(dhtpin,1)==SYSERR){
// 	// 	printf("gpio_write error\n");
// 	// 	return SYSERR;
// 	// }
// 	DELAY(40);
// 	gpio_read(dhtpin);
// 	// if(gpio_read(dhtpin)==SYSERR){
// 	// 	printf("gpio_read error\n");
// 	// 	return SYSERR;
// 	// }
// 	for (i = 0; i < MAXTIMINGS; i++ )
// 	{
// 		counter = 0;
// 		while(gpio_read(dhtpin)==laststate){
// 			counter++;
// 			if(counter==cntTrhd){
// 				break;
// 			}
// 		}
// 		laststate = gpio_read(dhtpin);
// 		if ( counter == cntTrhd )
// 			break;
// 		if ( (i >= 4) && (i % 2 == 0) )
// 		{
// 			dht11_dat[j / 8] <<= 1;
// 			if ( counter >  ioTrhd)
// 				dht11_dat[j / 8] |= 1;
// 			j++;
// 		}
// 	}
// 	if((j >= 40) &&
// 	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
// 	{
// 		for(i=0;i<4;i++){
// 			data[i]=dht11_dat[i];
// 		}
// 	}else{
// 		// for(i=0;i<4;i++){
// 		// 	data[i]=0;
// 		// }
// 		printf("check CRC error\n");
// 		return SYSERR;
// 	}
// 	return OK;
// }
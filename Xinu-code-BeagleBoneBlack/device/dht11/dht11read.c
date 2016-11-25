#include <xinu.h>

/*------------------------------------------------------------------------
 *  readdht11 -  Read from a dht11 device
 *------------------------------------------------------------------------
 */

//high level

devcall dht11read(
	struct dentry	*devptr,
	void *buf,
	uint32 count)
{
	
	uint32 dht11_dat[5] = { 0, 0, 0, 0, 0 };
	uint32	retval;
	char ret[count];
	retval=readdht11(devptr,(uint32 *)&dht11_dat);
	sprintf(ret,"%d %d %d %d",dht11_dat[0],dht11_dat[1],dht11_dat[2],dht11_dat[3]);
	memcpy((char *)buf,(char *)ret,count);
	return retval;
}
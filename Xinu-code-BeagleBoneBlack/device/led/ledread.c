#include <xinu.h>

devcall	ledread (
    struct dentry* devptr,
    void* buf,
    uint32 count)
{
	uint32 retval;
    uint32 data;
	char ret[count];
	retval = readled(devptr,(uint32 *)&data);
	sprintf(ret,"%d",data);
	memcpy((char *)buf,(char *)ret,count);
	return retval;
}
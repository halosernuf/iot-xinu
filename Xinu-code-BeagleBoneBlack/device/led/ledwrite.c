#include <xinu.h>

devcall	ledwrite (
    struct dentry* devptr,
    void* data,
    uint32 count)
{
	uint32 retval;
	retval = writeled(devptr,(uint32 *)data);
	// printf("%d \n", *((uint32* )data));
	return retval;
}
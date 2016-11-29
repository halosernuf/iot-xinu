#include <xinu.h>


devcall ledread(struct dentry* devptr, void* buff, uint32 count)
{
    uint32 len = 1;
    uint32 result[len];
    uint32 retval;
    char ret[count];
    retval = readled(devptr, (uint32 *)&result);
    sprintf(ret, "%d", result[0]);
    memcpy((char *)buff,(char *)ret,count);
    return retval;
}
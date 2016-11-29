#include <xinu.h>


devcall dht11read(struct dentry* devptr, void* buff, uint32 count)
{
    uint32 len = 4;
    uint32 result[len];
    uint32 retval;
    char ret[count];
    retval = readdht11(devptr, (uint32 *)&result);
    sprintf(ret, "%d %d %d %d", result[0],result[1],result[2],result[3]);
    memcpy((char *)buff,(char *)ret,count);
    return retval;
}
#include <xinu.h>


devcall tmp36read(struct dentry* devptr, void* buff, uint32 count)
{
    uint32 len = 1;
    uint32 result[len];
    uint32 retval;
    char ret[count];
    retval = readtmp36(devptr, (uint32 *)&result);
    result[0] = (((result[0]*1.8)/4095)-0.5)*100;
    sprintf(ret, "%d", result[0]);
    memcpy((char *)buff,(char *)ret,count);
    return retval;
}
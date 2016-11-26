/* ttyread.c - ttyread */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  tmp36read  -  Read character(s) from a tmp36 device (interrupts disabled)
 *------------------------------------------------------------------------
 */
//high level
devcall	tmp36read (
    struct dentry* devptr,
    void* buf,
    uint32 count)
{
    uint32 retval;
    uint32 data;
    char ret[count];
    retval = readtmp36(devptr,(uint32 *)&data);
    data = (((data*1.8)/4095)-0.5)*100;
    sprintf(ret,"%d",data);
    memcpy((char *)buf,(char *)ret,count);
    return retval;
}
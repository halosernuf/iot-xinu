#include <xinu.h>


devcall ledwrite(struct dentry* devptr, void* buff, uint32 count)
{
    uint32 retval;
    retval = writeled(devptr, (uint32*) buff);
    return retval;
}
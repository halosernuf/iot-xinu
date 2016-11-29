#include <xinu.h>

/*------------------------------------------------------------------------
 *  readled -  Read from a led device
 *------------------------------------------------------------------------
 */

//low level
devcall readled(
    struct dentry* devptr,
    uint32* data)
{
	uint32 GPIO;
	uint32 LEDPIN;
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
	LEDPIN=PINNUM%32;
	struct	gpio1_csreg *gpioptr;
	gpioptr = (struct gpio1_csreg *)GPIO;
	uint32 pin=1<<LEDPIN;
	// gpioptr->oe |= pin;
	*data= gpioptr->dataout>>LEDPIN & 1;
	return OK;
}
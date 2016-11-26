#include <xinu.h>

/*------------------------------------------------------------------------
 *  readled -  Read from a led device
 *------------------------------------------------------------------------
 */

//low level
devcall writeled(
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
	// printf("LEDPIN: %d\n",LEDPIN);
	// printf("GPIO: %x\n",GPIO);
	gpioptr->oe &= ~pin;
	if(*data){
		gpioptr->dataout |= pin;
	}else{
		gpioptr->dataout &= ~pin;
	}
	// printf("OE %x \n",gpioptr->oe);
	// printf("DATAOUT %x\n",gpioptr->dataout);
	// }
	return OK;
}
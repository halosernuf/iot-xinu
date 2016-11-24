#include <xinu.h>

syscall gpio_write(uint32 PINNUM,bool8 data){
	struct	gpio1_csreg *gpio1ptr;
	uint32 GPIO;
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
	gpio1ptr = (struct gpio1_csreg *)GPIO;
	uint32 pin=1<<(PINNUM%32);
	//set pin to output
	gpio1ptr->oe &= ~pin;
	if(data){
		gpio1ptr->dataout |= pin;
	}else{
		gpio1ptr->dataout &= ~pin;
	}
	// //set pin back to input
	// gpio1ptr->oe |= pin;
	return OK;
}

// void GPIO_1_WRITE(uint32 PINNUM,bool8 data){
// 	struct	gpio1_csreg *gpio1ptr;
// 	gpio1ptr = (struct gpio1_csreg *)GPIO_1;
// 	uint32 pin=1<<DHTPIN;
// 	//set pin to output
// 	gpio1ptr->oe &= ~pin;
// 	if(data){
// 		gpio1ptr->dataout |= pin;
// 	}else{
// 		gpio1ptr->dataout &= ~pin;
// 	}
// 	//set pin back to input
// 	gpio1ptr->oe |= pin;
// }

// void GPIO_2_WRITE(uint32 PINNUM,bool8 data){
// 	struct	gpio1_csreg *gpio1ptr;
// 	gpio1ptr = (struct gpio1_csreg *)GPIO_2;
// 	uint32 pin=1<<DHTPIN;
// 	//set pin to output
// 	gpio1ptr->oe &= ~pin;
// 	if(data){
// 		gpio1ptr->dataout |= pin;
// 	}else{
// 		gpio1ptr->dataout &= ~pin;
// 	}
// 	//set pin back to input
// 	gpio1ptr->oe |= pin;
// }

// void GPIO_3_WRITE(uint32 PINNUM,bool8 data){
// 	struct	gpio1_csreg *gpio1ptr;
// 	gpio1ptr = (struct gpio1_csreg *)GPIO_3;
// 	uint32 pin=1<<DHTPIN;
// 	//set pin to output
// 	gpio1ptr->oe &= ~pin;
// 	if(data){
// 		gpio1ptr->dataout |= pin;
// 	}else{
// 		gpio1ptr->dataout &= ~pin;
// 	}
// 	//set pin back to input
// 	gpio1ptr->oe |= pin;
// }
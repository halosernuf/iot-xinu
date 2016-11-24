#include <xinu.h>

bool8 gpio_read(uint32 PINNUM){
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
	//set pin to input
	gpio1ptr->oe |= pin;
	return gpio1ptr->datain>>PINNUM & 1;
}

// bool8 GPIO_0_READ(uint32 PINNUM){
// 	struct	gpio1_csreg *gpio1ptr;
// 	gpio1ptr = (struct gpio1_csreg *)GPIO_0;
// 	//SET PIN TO INPUT
// 	gpio1ptr->oe |= pin;
// 	return gpio1ptr->datain>>DHTPIN & 1;
// }

// bool8 GPIO_1_READ(uint32 PINNUM){
// 	struct	gpio1_csreg *gpio1ptr;
// 	gpio1ptr = (struct gpio1_csreg *)GPIO_1;
// 	//SET PIN TO INPUT
// 	gpio1ptr->oe |= pin;
// 	return gpio1ptr->datain>>DHTPIN & 1;
// }

// bool8 GPIO_2_READ(uint32 PINNUM){
// 	struct	gpio1_csreg *gpio1ptr;
// 	gpio1ptr = (struct gpio1_csreg *)GPIO_2;
// 	//SET PIN TO INPUT
// 	gpio1ptr->oe |= pin;
// 	return gpio1ptr->datain>>DHTPIN & 1;
// }

// bool8 GPIO_3_READ(uint32 PINNUM){
// 	struct	gpio1_csreg *gpio1ptr;
// 	gpio1ptr = (struct gpio1_csreg *)GPIO_3;
// 	//SET PIN TO INPUT
// 	gpio1ptr->oe |= pin;
// 	return gpio1ptr->datain>>DHTPIN & 1;
// }
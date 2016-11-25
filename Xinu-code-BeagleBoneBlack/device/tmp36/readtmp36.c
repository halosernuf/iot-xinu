/* ttyread.c - ttyread */

#include <xinu.h>
#include <delay.h>
/*------------------------------------------------------------------------
 *  tmp36read  -  Read character(s) from a tmp36 device (interrupts disabled)
 *------------------------------------------------------------------------
 */

//low level
devcall readtmp36(
    struct dentry* devptr,
    uint32* data)
{
    
    uint32 pin = (uint32)devptr->dvcsr;
    
    volatile uint32 *adcstat = (volatile uint32*)ADCSTAT;
    volatile uint32 *sysconfig = (uint32*)SYSCONFIG;
    volatile uint32 *ctrl = (volatile uint32*)CTRL;
    volatile uint32 *stepenable = (volatile uint32*)STEPENABLE;
    volatile uint32 *step1 = (volatile uint32*)(TS_CHARGE_STEPCONFIG+8);
    volatile uint32 *fifo0data = (volatile uint32*)FIFO0DATA;
    volatile uint32 *wake = (volatile uint32*)CM_WKUP_ADC_TSC_CLKCTRL;
    
    //wale up, provide clock, set 1-0 bit to 10
    *wake |= (1<<1);
    
    while (*wake != 2);
    
    //Reset SYSConfig Register to 0
    *sysconfig &= ~(0x0000000c);
    
    
    //wait adc to be idle
    while((*adcstat>>5&1)==1);
    
    //dissable adc_ss & set writtable
    *ctrl |= (1<<2);
    *ctrl &= ~(0x00000001);
    
    //enable step 1
    *stepenable |= (1<<1);
    
    //config step 1 and select AIN0(19-22 bit select pin AIN0-7
    //one shot and 16 average
    *step1 = (0x00000010);
    *step1 |= (pin<<19);
    
    //enable adc_ss
    *ctrl |= 0x00000005;
    
    //sleep(10);
    MDELAY(1);
    *data = *fifo0data;
    
    //power down adc_ss
    *ctrl |= (1<<2);
    *ctrl &= ~(0x00000001);
    //return d;
    
    return OK;
    
}



# Xinu source package readme

## 1. Update to original files

### /config/Configuration

Adding:
```
dht11:
    on dht11
        -i ionull   -o ionull   -c ionull
        -r dht11read    -g ionull   -p ionull
        -w ionull   -s ionull   -n ionull
        -intr ionull
        
tmp36:
    on tmp36
        -i ionull   -o ionull   -c ionull
        -r tmp36read    -g ionull   -p ionull
        -w ionull   -s ionull   -n ionull
        -intr ionull
```

Addding:
```
   /* DHT11 devices */
   DHT11_0 is dht11 on dht11 csr 39
   
   /* TMP36 devices */
   TMP36_0 is tmp36 on tmp36 csr 0
```

### /include/xinu.h

Adding:
```
#include <adc.h>
```

### /include/prototypes.h

Adding:
```
/* in file readdht11.c */
extern devcall readdht11(struct dentry *, uint32 *);

/* in file dht11read.c */
extern devcall dht11read(struct dentry *, void *, uint32);

/* in file readtmp36.c */
extern devcall readtmp36(struct dentry *, uint32 *);

/* in file tmp36read.c */
extern devcall tmp36read (struct dentry*, void *, uint32);
```

### /include/gpio.h

Changing to:
```
/* gpio.h */

#define GPIO_0 0x44E07000
#define GPIO_1 0x4804C000
#define GPIO_2 0x481AC000
#define GPIO_3 0x481AE000

struct gpio1_csreg {
    volatile uint32 revision;   //0-4
    volatile uint32 res1[3];    //4-10
    volatile uint32 sysconfig;  //10-14
    volatile uint32 res2[3];    //14-20
    volatile uint32 eoi;        //20-24
    volatile uint32 irqstatus_raw_0;//24-28
    volatile uint32 irqstatus_raw_1;//28-2c
    volatile uint32 irqstatus_0;    //2C-30
    volatile uint32 irqstatus_1;    //30-34
    volatile uint32 irqstatus_set_0;//34-38
    volatile uint32 irqstatus_set_1;//38-3c
    volatile uint32 irqstatus_clr_0;//3c-40
    volatile uint32 irqstatus_clr_1;//40-44
    volatile uint32 irqwaken_0; //44-48
    volatile uint32 irqwaken_1; //48-4c
    volatile uint32 res3[50];   //4c-114
    volatile uint32 sysstatus;  //114-118
    volatile uint32 res4[6];    //118-130
    volatile uint32 ctrl;       //130-134
    volatile uint32 oe;         //134-138
    volatile uint32 datain;     //138-13c
    volatile uint32 dataout;        //13c-140
    volatile uint32 leveldetect0;//140-144
    volatile uint32 leveldetect1;//144-148
    volatile uint32 risingdetect;//148-14c
    volatile uint32 fallingdetect;//14c-150
    volatile uint32 debouncenable;//150-154
    volatile uint32 debouncingtime;//154-15c
    volatile uint32 res5[13];   //15c-190
    volatile uint32 cleardataout;   //190-194
    volatile uint32 setdataout;     //194-198
};
```

### /system/main.c

Adding:

**process** udp_routine()

Description: The process to communicate with the server, it actively waiting for the request from server and return the data as required. 

**process** dht11_routine()

Description: The process to monitoring the DHT11 devices 

**process** tmp36_routine()

Description: The process to monitoring the TMP36 devices 

## 2. New files

### /device/dht11/readdht11.c

* devcall readdht11(struct dentry *, uint32 *)
    - Low level driver
    - Read from a DHT11 device
    - Sending start signal 
    - Receving response
    - Write 5 integer to the buffer provided

### /device/tmp36/readtmp36.c

* devcall readtmp36(struct dentry *, uint32 *);
    - Low level driver
    - Using TSC_ADC to read from a TMP36 sensor
    - Write 1 integer to the buffer provided

### /include/adc.h

Include the address information of TSC_ADC
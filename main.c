#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <moves.h>
#include <camera/po8030.h>
#include <chprintf.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <scan.h>
#include <pi_regulator.h>
#include <process_image.h>

#define TRACKING_BUFFER_SIZE		180

void SendUint8ToComputer(uint8_t* data, uint16_t size) 
{
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)"START", 5);
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)&size, sizeof(uint16_t));
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)data, size);
}

void SendUint16ToComputer(uint16_t* data, uint16_t size)
{
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)"START", 5);
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)&size, sizeof(uint16_t));
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)data, sizeof(uint16_t)*size);
}


static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

int main(void)
{
//	volatile uint8_t tab[NB_TREES_MAX] = {0};
//	volatile uint16_t d;

    halInit();
    chSysInit();
    mpu_init();

    //starts the serial communication
    serial_start();
    //start the USB communication
    usb_start();
    //starts the camera
    dcmi_start();
	po8030_start();
	//inits the motors
	motors_init();
	VL53L0X_start();

	int* tab = scaning();
	//	SendUint16ToComputer(tof_value, TRACKING_BUFFER_SIZE);

	chThdSleepMilliseconds(1000);

	for (int i=0; i<1; i++)
	{
		if (i != 0)
		{
			turn(RIGHT, MIDDLE_SPEED, tab[i]);
			pluck();
		}
	}

	while(1)
	{
		// d = VL53L0X_get_dist_mm();
		chThdSleepMilliseconds(100);
	}

}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

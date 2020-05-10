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
    halInit();
    chSysInit();
    mpu_init();
    VL53L0X_start();
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

	int*tab = scaning();
	//	SendUint16ToComputer(tof_value, TRACKING_BUFFER_SIZE);
	//stars the threads for the processing of the image
	process_image_start();

	chThdSleepMilliseconds(1000);
	volatile uint8_t color_test = 20;

	//scan(RIGHT, MIDDLE_SPEED, 180);

	// pluck();
	// deposit();

	for (int i=0; i<NB_TREES_MAX; i++)
	{
		if (tab[2*i] != 0)
		{
			if (i==0) turn(RIGHT, MIDDLE_SPEED, tab[2*i]);
			else turn(RIGHT, MIDDLE_SPEED, tab[2*i]-tab[2*(i-1)]);

			chThdSleepMilliseconds(500);
			motors_set_pos(abs(0.075*tab[2*i+1] - 6), abs(0.075*tab[2*i+1] - 6), MIDDLE_SPEED, MIDDLE_SPEED);
			chThdSleepMilliseconds(500);
			pluck();
			motors_set_pos(abs(0.075*tab[2*i+1] - 6), abs(0.075*tab[2*i+1] - 6), -MIDDLE_SPEED, -MIDDLE_SPEED);
		}
		color_test = get_color();
	}

	while(1)
	{
		chThdSleepMilliseconds(100);
	}

}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

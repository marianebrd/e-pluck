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

	//stars the threads for the processing of the image
	process_image_start();
	po8030_set_ae(0); //Enables/disables auto exposure ; 1 to enable auto exposure
	//po8030_set_rgb_gain(1, 1, 1); //Sets white balance red, green, blue gain
	po8030_set_awb(0); //Enable/disable auto white balance ; 1 to enable auto white balance


	volatile uint8_t color_test = 20;

	//scan(RIGHT, MIDDLE_SPEED, 180);

	// pluck();
	// deposit();

	while(1)
	{
		color_test = get_color();
		chThdSleepMilliseconds(500);
	}
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

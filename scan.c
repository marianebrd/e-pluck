#include "moves.h"
#include <ch.h>
#include <hal.h>
#include <stdlib.h>
#include <stdbool.h>
#include "motors.h"
#include "leds.h"

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"
#include <sensors/VL53L0X/VL53L0X.h>
#include "scan.h"

int j = 0;
volatile uint16_t object_pos[SCAN_ANGLE] = {0};

int scaning(){

	for (int i=0; i<=SCAN_ANGLE; i++)
	{
		chThdSleepMilliseconds(100);
		turn(RIGHT, MIDDLE_SPEED, SCAN_RATE);  // tourne
		object_pos[j] = VL53L0X_get_dist_mm();  // mesure TOF
		j++;
	}
	return object_pos;
}

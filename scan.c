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
uint16_t tof_value[SCAN_ANGLE] = {0};
int tree_angle[NB_TREES_MAX] = {0};

int* scaning(){

	j = 0;

	for (int i=0; i<SCAN_ANGLE/SCAN_RATE; i++)
	{
		chThdSleepMilliseconds(100);
		turn(RIGHT, LOW_SPEED, SCAN_RATE);  // tourne
		tof_value[j] = VL53L0X_get_dist_mm();  // mesure TOF
		j++;
	}

	j = 0;

	// trouve les minimums locaux des mesures du TOF
	for (int i=3; i<SCAN_ANGLE/SCAN_RATE-3; i++)
	{
		if( (tof_value[i] < 200) && // exclut les minimums calculés sur l'arrière-plan
			(tof_value[i] < tof_value[i-1]) && (tof_value[i] < tof_value[i+1]) &&
			(tof_value[i] < tof_value[i-2]) && (tof_value[i] < tof_value[i+2]) &&
			(tof_value[i] < tof_value[i-3]) && (tof_value[i] < tof_value[i+3]) )
		{
			tree_angle[j] = i*SCAN_RATE;
			j++;
		}
	}
	// go back to home position
	chThdSleepMilliseconds(1000);
	turn(LEFT, MIDDLE_SPEED, SCAN_ANGLE);
	return tree_angle;
}

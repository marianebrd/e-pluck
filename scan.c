#include "moves.h"
#include <ch.h>
#include <hal.h>
#include <stdlib.h>
#include <stdbool.h>
#include "motors.h"
#include "leds.h"
#include <main.h>

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"
#include <sensors/VL53L0X/VL53L0X.h>
#include "scan.h"

uint8_t j = 0;
uint16_t tof_value[NB_LIGNS_TOF_TAB] = {0};
static uint16_t tree_angle[NB_TREES_MAX*2] = {0};

uint16_t* scaning(){

	j = 0;

	for (uint8_t i=0; i < (uint8_t) ((float)(SCAN_ANGLE)/SCAN_RATE); i++)
	{
		chThdSleepMilliseconds(SHORT_PAUSE);
		turn(RIGHT, MIDDLE_SPEED, SCAN_RATE);  // tourne d'un a-coup
		tof_value[j] = VL53L0X_get_dist_mm();  // mesure TOF
		j++;
	}

	j = 0;

	// trouve les minimums locaux des mesures du TOF
	for (uint8_t i=3; i<SCAN_ANGLE/SCAN_RATE-3; i++)
	{
		if( (tof_value[i] < TREE_DISTANCE_MAX) && // exclut les minimums calculés sur l'arrière-plan
			(tof_value[i] <= tof_value[i-1]) && (tof_value[i] <= tof_value[i+1]) &&
			(tof_value[i] <= tof_value[i-2]) && (tof_value[i] <= tof_value[i+2]) &&
			(tof_value[i] <= tof_value[i-3]) && (tof_value[i] <= tof_value[i+3]) )
		{
			if ((j == 0) || ((i*SCAN_RATE - tree_angle[2*(j-1)]) > ANGLE_MIN_BETWEEN_TREES))
			{
			tree_angle[2*j] = i*SCAN_RATE; // stocke la valeur de l'angle
			tree_angle[2*j+1] = tof_value[i]; // stocke la distance entre l'arbre et le robot (TOF)
			j++;
			}
		}
	}
	// go back to home position
	chThdSleepMilliseconds(LONG_PAUSE);
	turn(LEFT, MIDDLE_SPEED, SCAN_ANGLE);
	return tree_angle;
}

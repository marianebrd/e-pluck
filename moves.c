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

static int l_pos;
static int r_pos;

void motors_set_pos(int l_distance, int r_distance, int l_speed, int r_speed){
	// this function allows to control both motors at the same time

	// transforms from cm to nb steps
	l_distance *= NSTEP_ONE_TURN / WHEEL_PERIMETER;
	r_distance *= NSTEP_ONE_TURN / WHEEL_PERIMETER;

	// flags initialization
	l_pos = 0;
	r_pos = 0;

	// transforms from cm/s into step/s
	l_speed *= NSTEP_ONE_TURN / WHEEL_PERIMETER;
	r_speed *= NSTEP_ONE_TURN / WHEEL_PERIMETER;

	// sets speed of both motors
	left_motor_set_speed(l_speed);
	right_motor_set_speed(r_speed);

	// stay in this loop until position is reached
	while ((abs(l_pos) < l_distance) | (abs(r_pos) < r_distance))
	{
		l_pos = left_motor_get_pos();
		r_pos = right_motor_get_pos();

		// left motor stopped if his position reached
		if (abs(l_pos) >= l_distance){
			left_motor_set_speed(0);
		}
		// right motor stopped if his position reached
		if (abs(r_pos) >= r_distance){
			right_motor_set_speed(0);
		}
	}

	// re initialization
	left_motor_set_pos(0);
	right_motor_set_pos(0);
}

void turn(bool direction, int turn_speed, float turn_angle) {
	// this function detects object around the e-puck

	// flags initialization
	l_pos = 0;
	r_pos = 0;

	// transforms from cm/s into step/s
	turn_speed *= NSTEP_ONE_TURN / WHEEL_PERIMETER;

	if (direction)
	{
		left_motor_set_speed(turn_speed);
		right_motor_set_speed(-turn_speed);
	}
	else {
		left_motor_set_speed(-turn_speed);
		right_motor_set_speed(turn_speed);
	}

	// stay in this loop until position is reached
	while (	(abs(l_pos) < turn_angle*ANGLE_TO_NSTEP) |
			(abs(r_pos) < turn_angle*ANGLE_TO_NSTEP) )
	{
		// tracking
		l_pos = left_motor_get_pos();
		r_pos = right_motor_get_pos();

		// left motor stopped if his position reached
		if (abs(l_pos) >= turn_angle*ANGLE_TO_NSTEP){
			left_motor_set_speed(0);
		}

		// right motor stopped if his position reached
		if (abs(r_pos) >= turn_angle*ANGLE_TO_NSTEP){
			right_motor_set_speed(0);
		}
	}

	//re initialization
	left_motor_set_pos(0);
	right_motor_set_pos(0);
}

void pluck(void) {
	motors_set_pos(5, 5, HIGH_SPEED, HIGH_SPEED);
	chThdSleepMilliseconds(1000);
	motors_set_pos(5, 5, -MIDDLE_SPEED, -MIDDLE_SPEED);
}

void deposit(void) {
	motors_set_pos(3, 3, -HIGH_SPEED, -HIGH_SPEED);
	motors_set_pos(3, 3, HIGH_SPEED, HIGH_SPEED);
	motors_set_pos(3, 3, -HIGH_SPEED, -HIGH_SPEED);
	motors_set_pos(3, 3, HIGH_SPEED, HIGH_SPEED);
	motors_set_pos(3, 3, -HIGH_SPEED, -HIGH_SPEED);
	motors_set_pos(3, 3, HIGH_SPEED, HIGH_SPEED);
}

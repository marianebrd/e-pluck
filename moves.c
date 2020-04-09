#include "moves.h"
#include <ch.h>
#include <hal.h>
#include <stdlib.h>
#include "motors.h"
#include "leds.h"

void motors_set_pos(int l_distance, int r_distance, int l_speed, int r_speed){

	int l_pos = 0;
	int r_pos = 0;

	left_motor_set_speed(l_speed);
	right_motor_set_speed(r_speed);

	while ((abs(l_pos) < l_distance) | (abs(r_pos) < r_distance))
	{
		l_pos = left_motor_get_pos();
		r_pos = right_motor_get_pos();

		if (abs(l_pos) >= l_distance){
			left_motor_set_speed(0);
		}

		if (abs(r_pos) >= r_distance){
			right_motor_set_speed(0);
		}
	}

	left_motor_set_pos(0);
	right_motor_set_pos(0);
}

int pluck() {
	motors_set_pos(200, 200, 300, 300);
	chThdSleepMilliseconds(1000);
	motors_set_pos(200, 200, 1100, 1100);
	chThdSleepMilliseconds(1000);
	motors_set_pos(400, 400, -400, -400);
	chThdSleepMilliseconds(1000);
	motors_set_pos(323, 323, 400, -400);
	chThdSleepMilliseconds(1000);
	motors_set_pos(1200, 1200, 400, 400);
	chThdSleepMilliseconds(1000);
}

int deposit() {
	motors_set_pos(200, 200, 700, 700);
	motors_set_pos(200, 200, -700, -700);
	motors_set_pos(200, 200, 700, 700);
	motors_set_pos(200, 200, -700, -700);
	motors_set_pos(200, 200, 700, 700);
	motors_set_pos(200, 200, -700, -700);
}

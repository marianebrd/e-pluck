#include "motors.h"
#include "moves.h"

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
	motors_set_pos(200, 200, 1100, 1100);
	motors_set_pos(200, 200, -1100, -1100);
	motors_set_pos(200, 200, 1100, 1100);
	motors_set_pos(200, 200, -1100, -1100);
	motors_set_pos(200, 200, 1100, 1100);
	motors_set_pos(200, 200, -1100, -1100);
}

#ifndef MOVES_H
#define MOVES_H

#include <stdlib.h>
#include <stdbool.h>

#define STOP_SPEED					0 // [cm/s]
#define LOW_SPEED					2 // [cm/s]
#define MIDDLE_SPEED				4 // [cm/s]
#define HIGH_SPEED					10 // [cm/s]
#define MAX_SPEED					15 // [cm/s]
#define ONE_TURN_DEGREES			360 // degrees
#define NSTEP_ONE_TURN      		1000 // number of step for 1 turn of the motor
#define NSTEP_ONE_EL_TURN   		4  //number of steps to do 1 electrical turn
#define NB_OF_PHASES        		4  //number of phases of the motors
#define WHEEL_PERIMETER     		12.95f // [cm]
#define NSTEP_FOR_360_ROTATION 		1308 // number of steps to do a rotation of 360�
#define RIGHT						1
#define LEFT						0
#define PI                			3.1415926536f
#define WHEEL_DISTANCE     			5.35f //cm
#define PERIMETER_EPUCK   			(PI * WHEEL_DISTANCE)
#define ANGLE_TO_NSTEP				NSTEP_ONE_TURN*PERIMETER_EPUCK/(WHEEL_PERIMETER*ONE_TURN_DEGREES) // converts angle into number of steps
#define PUCK_DISTANCE				6 // cm
#define DEPOSIT_DISTANCE_FW			4 // cm (forward)
#define DEPOSIT_DISTANCE_BW			5 // cm (backwards)

void motors_set_pos(float l_distance, float r_distance, int l_speed, int r_speed);
void turn(bool direction, int turn_speed, float turn_angle);
void pluck(void);
void deposit(void);
void victory(void);

#endif /* MOVES_H */

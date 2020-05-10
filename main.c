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
#include <leds.h>

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
	// initialisations
    halInit();
    chSysInit();
    mpu_init();
    VL53L0X_start();
    serial_start();    //starts the serial communication
    usb_start();    //start the USB communication
    dcmi_start();    //starts the camera
	po8030_start();
	motors_init();	//inits the motors
	process_image_start();

	// actions
	motors_set_pos(DISTANCE_TO_BASKET - FRAME, DISTANCE_TO_BASKET - FRAME, MIDDLE_SPEED, MIDDLE_SPEED); // pénètre dans le champ
	turn(LEFT, MIDDLE_SPEED, QUARTER_TURN); // quart de tour gauche
	chThdSleepMilliseconds(LONG_PAUSE);
	uint16_t*tab = scaning(); // tab est le tableau qui contient l'angle et la distance de chaque arbre obtenus par la fct scaning (pointeur)
	chThdSleepMilliseconds(LONG_PAUSE);

	int j = 0;
	for (int i=0; i<NB_TREES_MAX; i++)
	{
		if (tab[(2*i)+1] != 0) // si la valeur est 0, c'est qu'il n'y a pas d'arbre
		{
				if (i==0) // si pas d'arbre
					turn(RIGHT, MIDDLE_SPEED, ANGLE_CORR*tab[2*i]);
				else
					turn(RIGHT, MIDDLE_SPEED, ANGLE_CORR*(tab[2*i]-tab[2*(i-1)])); // pointe l'arbre suivant
				chThdSleepMilliseconds(LONG_PAUSE);
				motors_set_pos(abs(MOVE_SLOPE_CORR*tab[2*i+1] - MOVE_OFFSET_CORR), abs(MOVE_SLOPE_CORR*tab[2*i+1] - MOVE_OFFSET_CORR), MIDDLE_SPEED, MIDDLE_SPEED); // approche l'arbre
				chThdSleepMilliseconds(LONG_PAUSE);
				if ((get_color() == YELLOW) || (get_color() == RED)) // si la pome est colorée, on la cueille !
					pluck();
				motors_set_pos(abs(MOVE_SLOPE_CORR*tab[2*i+1] - MOVE_OFFSET_CORR), abs(MOVE_SLOPE_CORR*tab[2*i+1] - MOVE_OFFSET_CORR), -MIDDLE_SPEED, -MIDDLE_SPEED); // retour au centre
				j++;
		}
	}

	turn(RIGHT, MIDDLE_SPEED, (THREE_QUARTERS_TURN - tab[2*(j-1)]*ANGLE_CORR)); // pointe le panier
	chThdSleepMilliseconds(LONG_PAUSE);
	motors_set_pos(DISTANCE_TO_BASKET, DISTANCE_TO_BASKET, MIDDLE_SPEED, MIDDLE_SPEED); // approche le panier
	chThdSleepMilliseconds(LONG_PAUSE);

	if (j != 0)
		deposit(); // dépose les pommes

	victory(); // annonce la fin

	while(1)
	{
		chThdSleepMilliseconds(LONG_PAUSE);
	}
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

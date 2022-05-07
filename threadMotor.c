/*
 * threadMotor.c
 *
 *  Created on: 6 mai 2022
 *      Author: Louis&Valentin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <motors.h>
#include <leds.h>
#include <usageMotors.h>
#include <actionUser.h>
#include "threadMotor.h"
#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <msgbus/messagebus.h>

static THD_WORKING_AREA(waThdExplore, 256);
static THD_FUNCTION(ThdExplore, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    while(1){
    	// calcul de la distance
		int distance = VL53L0X_get_dist_mm();
		chprintf((BaseSequentialStream *)&SD3, " Distance= %d \r\n", // print the distance mesured in the serial monitor
				distance);
		if(distance > 150 ){
			go_straight(MOTOR_SPEED_LIMIT);
		}
		else if(distance<150 && distance>50){
			go_straight(MOTOR_SPEED_LIMIT/2);
		}
		else{
			stopMotors();
			leds_stop(500);
			int distG = 0; int distD=0;
			// capte la distance a gauche
			turn_left(MOTOR_SPEED_LIMIT/3);
			chThdSleepMilliseconds(50);
			distG = VL53L0X_get_dist_mm();
			// capte la distance a droite
			u_turn(MOTOR_SPEED_LIMIT/3);
			chThdSleepMilliseconds(50);
			distD = VL53L0X_get_dist_mm();
			// oriente le robot dans la bonne direction
			if (distG < 100 && distD < 100){
				turn_right(MOTOR_SPEED_LIMIT/3);
				chThdSleepMilliseconds(50);
			}
			else if(distD < distG){
				u_turn(MOTOR_SPEED_LIMIT/3);
				chThdSleepMilliseconds(50);
			}
			else{
				chThdSleepMilliseconds(50);
			}
		}
		chThdSleepMilliseconds(200);
    }


}

void explorationLabyrinthe(void)
{
	chThdCreateStatic(waThdExplore, sizeof(waThdExplore), NORMALPRIO+1, ThdExplore, NULL);
}



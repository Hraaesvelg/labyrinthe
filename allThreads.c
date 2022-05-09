/*
 * allThreads.c
 *
 *  Created on: 7 mai 2022
 *      Author: Louis&Valentin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include <motors.h>
#include <leds.h>
#include <selector.h>
#include <sensors/proximity.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <msgbus/messagebus.h>
#include <usageMotors.h>
#include <actionUser.h>
#include <chprintf.h>
#include <sensors/imu.h>
#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <allthreads.h>
#include <color.h>

static uint8_t running = ACTIVE;

static THD_WORKING_AREA(waThd_motor, 256);
static THD_FUNCTION(Thd_motor, arg) { // @suppress("No return")

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    set_led(1,1);
    chprintf((BaseSequentialStream *)&SD3, " Running= %d \r\n",running);
    int selector = 1;
    while(selector == 1){
    	selector = get_selector();
    	//calcul de la distance
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

			//test if the obstacle is red (ie. if the obstacle is a target)
			// if the target is located it will be attacked
			int color = get_color_from_col();
			if(color == 1){
				// procedure d'attaque
				attack_target(distance);
			}
			else{
				// capte la distance a gauche
				turn_left(MOTOR_SPEED_LIMIT/3);
				chThdSleepMilliseconds(50);
				distG = VL53L0X_get_dist_mm();

				// capte la distance a droite
				u_turn(MOTOR_SPEED_LIMIT/3);
				chThdSleepMilliseconds(50);
				distD = VL53L0X_get_dist_mm();

				clear_leds();

				// oriente le robot dans la bonne direction
				if (distG < 100 && distD < 100){
					set_led(1,1);
					turn_right(MOTOR_SPEED_LIMIT/3);
					stopMotors();
					chThdSleepMilliseconds(500);
				}
				else if(distD < distG){
					set_led(3,1);
					u_turn(MOTOR_SPEED_LIMIT/3);
					stopMotors();
					chThdSleepMilliseconds(500);
				}
				else{
					stopMotors();
					chThdSleepMilliseconds(500);
				}
			}
		}
		clear_leds();
		chThdSleepMilliseconds(200);
    }
}


void thread_motor(void)
{
	chprintf((BaseSequentialStream *)&SD3, "Thread Motor = %d \r\n", 1);
	chThdCreateStatic(waThd_motor, sizeof(waThd_motor), NORMALPRIO, Thd_motor, NULL);
}

void enable_thread_motor(void){
	running = ACTIVE;
}

void disable_thread_motor(void){
	running = NONACTIVE;
}

uint8_t get_running_motor(void){
	return running;
}


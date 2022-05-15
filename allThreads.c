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
#include <button.h>
#include <audio/microphone.h>
#include "audio/play_melody.h"
#include "audio/play_sound_file.h"




static THD_WORKING_AREA(waThd_main_function, 2048);
static THD_FUNCTION(Thd_main_function, arg) { // @suppress("No return")

	chRegSetThreadName(__FUNCTION__);
    (void)arg;


    while(1){
			// Capter la valeur du selecteur
		int selector = get_selector()/4;
		switch (selector){
			case 0: // Labyrinthe
				if(button_get_state()){
					explore_maze();
				}
				break;
			case 1:
				if(button_get_state()){
					ckeck_robot();
					chThdSleepMilliseconds(100);
				}
				break;
			case 2:
				if(button_get_state()){
					chThdSleepMilliseconds(1000);
					set_rgb_all_leds(RED);
					chThdSleepMilliseconds(1000);
					set_rgb_all_leds(BLUE);
					chThdSleepMilliseconds(1000);
					set_rgb_all_leds(GREEN);
					chThdSleepMilliseconds(1000);
					set_rgb_all_leds(BLACK);
					spin(SPEED_FAST, 5);
					playMelody(SEVEN_NATION_ARMY, ML_SIMPLE_PLAY, NULL);
					chThdSleepMilliseconds(1000);

				}
				break;
			case 3:
				if (button_get_state()){
					set_body_led(0);
					int color = get_main_color();
					set_rgb_all_leds(color);
				}
				break;
			default:
				up_leds_blink(200);
				break;
		}
	stopMotors();
    }
}


void main_thread_start(void)
{
	chThdCreateStatic(waThd_main_function, sizeof(waThd_main_function), NORMALPRIO, Thd_main_function, NULL);
}



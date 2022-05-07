/*
 * actionUser.c
 *
 *  Created on: 5 mai 2022
 *      Author: Louis&Valentin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <actionUser.h>
#include <leds.h>

void blink_all_leds(int number){
	clear_leds();
	int8_t count = 0;
	while(count <= number){
		for(int8_t i=0; i<=3; i++){
			set_led(i, 1);
			chThdSleepMilliseconds(200);
			clear_leds();
		}
		count += 1;
	}
	count = 0;
	while(count <= number){
		set_body_led(1);
		set_front_led(0);
		chThdSleepMilliseconds(200);
		set_body_led(0);
		set_front_led(1);
		chThdSleepMilliseconds(200);
		count +=1;
	}
	set_front_led(0);
	clear_leds();
}

void leds_think(int number){
	clear_leds();
	int8_t count = 0;
	while(count <= number){
		for(int8_t i=0; i<=3; i++){
			set_led(i, 1);
			chThdSleepMilliseconds(200);
			clear_leds();
		}
		count += 1;
	}
}

void leds_stop(int time)
{
	clear_leds();
	for(int8_t i=1; i<=3; i++){
		set_led(i, 1);
	}
	chThdSleepMilliseconds(time);
}

void up_leds_blink(int time){
	clear_leds();
	for(int8_t i=0; i<=3; i++){
			set_led(i, 1);
	}
	chThdSleepMilliseconds(time);
	clear_leds();
	chThdSleepMilliseconds(time);

}


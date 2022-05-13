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
#include <usageMotors.h>
#include <motors.h>
#include <leds.h>
#include <sensors/proximity.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include "ch.h"
#include <chprintf.h>
#include <color.h>



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

void all_leds_red(void){
	for(int i = 0; i<4;i++){
		set_led(i,1);
	}
	set_front_led(1);
}

void all_leds_off(void){
	for(int i = 0; i<4;i++){
		set_led(i,0);
	}
	set_front_led(0);
}

void attack_target(int dist2target){
	all_leds_red();
	move_str_dist(dist2target + 2, 1000);
	all_leds_off();
	set_body_led(1);
	chThdSleepMilliseconds(300);
	set_body_led(0);
}

// Jeu de lumière avec les led rouge/body/led
void initialisationLeds(int ite){
	clear_leds();
	int8_t compt = 0;
	while(compt <= ite){
		for(int8_t i=0; i<=3; i++){
			set_led(i, 1);
			chThdSleepMilliseconds(200);
			clear_leds();
		}
		compt += 1;
	}
	compt = 0;
	while(compt <= ite){
		set_body_led(1);
		set_front_led(0);
		chThdSleepMilliseconds(200);
		set_body_led(0);
		set_front_led(1);
		chThdSleepMilliseconds(200);
		compt +=1;
	}
	set_front_led(0);
	clear_leds();
}

void follow_wall(void){

}

int front_dist_ir(void){
	int dist = (get_prox(0)+get_prox(7))/2;
	return dist;
}

void explore_maze(void){
	//explore_maze();
	int size_tab = 200;
	int pos_tab = 0;
	int tab_direction[1000] = { 0 };
	int distance = 0;
	int str_count = 0;
	chprintf((BaseSequentialStream *)&SD3, "Entrée parcours labyrinthe = %d \r\n", 1);
	left_motor_set_pos(0);
	while((pos_tab < size_tab)&&(str_count < 20)){
		set_rgb_led(0,0,0,30);
		int front_dist = front_dist_ir();
		chprintf((BaseSequentialStream *)&SD3, "DISTANCE = %d \r\n", front_dist_ir());
		int proxD = get_prox(2);
		int proxG = get_prox(5);

		if(front_dist < 400){
			int distdroite = get_prox(1);
			int distgauche = get_prox(6);
			if (distdroite > 300){
				deviation_robot(250, 100);
				chThdSleepMilliseconds(100);
				distdroite = get_prox(1);
			}
			if (distgauche > 300){
				deviation_robot(100, 200);
				chThdSleepMilliseconds(100);
				distgauche = get_prox(6);
			}
			stopMotors();
			go_straight(500);
			/*if((proxD < 150)&&(proxG > 130)){
				chprintf((BaseSequentialStream *)&SD3, "LIBRE A DROITE = %d \r\n", proxD, proxG);
				stopMotors();
				chThdSleepMilliseconds(500);
				move_str_dist(2,300);
				stopMotors();
				turn_right(300);
				stopMotors();
			}*/
		}
		else{
			stopMotors();
			tab_direction[pos_tab] = left_motor_get_pos();
			int proxD = get_prox(2);
			int proxG = get_prox(5);
			if(proxD < 150){
				turn_right(300);
				tab_direction[pos_tab+1] = 1;
				stopMotors();
			}
			else if(proxG < 150)
			{
				turn_left(300);
				tab_direction[pos_tab] = 2;
				stopMotors();
			}
			else{
				u_turn(300);
				tab_direction[pos_tab] = 3;
				stopMotors();
			}
			left_motor_set_pos(0);
		}
		pos_tab += 2;
		chThdSleepMilliseconds(100);
	}
	for(int i = pos_tab; i >= 0; i--){
		chprintf((BaseSequentialStream *)&SD3, "position = %d orientation = %d \r\n", i, tab_direction[i]);
	}
}

void EP_call_homme(int tableau[]){
	size_t size_tbl = sizeof(tableau);
	for(size_t i = 0 ; i < size_tbl; i+=2){
		move_str_from_pos(tableau[i], 500);
		if(tableau[i+1] == RIGHT)
		{
			turn_left(500);
			stopMotors();
		}
		else if(tableau[i+1] == LEFT)
		{
			turn_right(500);
			stopMotors();
		}
		else{
			u_turn(500);
		}
	}
	stopMotors();
	set_body_led(1);
	u_turn(500);
	show_panic(300, 3);
}

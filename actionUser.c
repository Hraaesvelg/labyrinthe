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
	//recherche du mur
	set_body_led(1);
	turn_right(500);
	int distance = 100;
	while(distance > 40){
		distance = VL53L0X_get_dist_mm();
		go_straight(200);
	}
	stopMotors();
	turn_left(200);
	int dist_to_wall = get_prox(3);
	set_body_led(0);
	while(1){
		int dist = get_prox(3);
		chprintf((BaseSequentialStream *)&SD3, "distance = %d \r\n", dist);
		if ((dist > dist_to_wall + side_margin)&&(dist < dist_to_wall - side_margin)){
			go_straight(400);
			set_led(1,1);
		}
		else if(dist < dist_to_wall + side_margin){
			stopMotors();
			turn_right(400);
			set_led(1,0);
		}
		else if(dist > dist_to_wall - side_margin){
			stopMotors();
			turn_left(400);
			set_led(1,0);
		}

	}
}

void explore_maze(void){
	//explore_maze();
	int size_tab = 200;
	int pos_tab = 0;
	int tab_direction[1000] = { 0 };
	int distance = 0;
	int str_count = 0;
	chprintf((BaseSequentialStream *)&SD3, "Entrée parcours labyrinthe = %d \r\n", 1);
	while((pos_tab < size_tab)&&(str_count < 10)){
		distance = VL53L0X_get_dist_mm();
		chprintf((BaseSequentialStream *)&SD3, "Distance = %d \r\n", distance);
		if(distance > 100){ // Avancer si rien ne bloque le passage
			move_str_dist(5, 500);
			stopMotors();
			//tab_direction[pos_tab] = 0;
			str_count++;
		}
		turn_right(500);
		int distD = VL53L0X_get_dist_mm();
		if(distD > 100){ //tourner a droite dans la majorité des cas
			str_count = 0;
			//tab_direction[pos_tab] = 1;
		}
		else if((distD < 70)&&(distance < 70)){ // Tourner a gauche si le chemin est bloqué en face et a droite
			turn_left(500);
			stopMotors();
			turn_left(500);
			stopMotors();
			str_count = 0;
			int DistG = VL53L0X_get_dist_mm();
			if ((DistG< 70)&&(distance < 70)&&(distD < 70)){
				turn_left(500);
				stopMotors();
				//tab_direction[pos_tab] = 3;
			}
			else{
				//tab_direction[pos_tab] = 2;
			}
		}
		else{ // Continuer tout droit
			turn_left(500);
			stopMotors();
		}
		pos_tab ++;
	}
	/*for(int i = pos_tab; i >= 0; i--){
		chprintf((BaseSequentialStream *)&SD3, "position = %d orientation = %d \r\n", i, tab_direction[i]);
	}*/
}

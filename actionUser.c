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
#include "audio/audio_thread.h"
#include "audio/play_melody.h"
#include "audio/play_sound_file.h"
#include <sensors/battery_level.h>
#include <sensors/mpu9250.h>
#include <sensors/imu.h>
#include "button.h"

#define size_tab 100

static int tab_path[size_tab] = {0};

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
	set_rgb_led(LED2,10,0,0);
	set_rgb_led(LED4,10,0,0);
	set_rgb_led(LED6,10,0,0);
	set_rgb_led(LED8,10,0,0);
	set_front_led(1);
}

void all_leds_off(void){
	for(int i = 0; i<4;i++){
		set_led(i,0);
	}
	set_rgb_led(LED2,0,0,0);
	set_rgb_led(LED4,0,0,0);
	set_rgb_led(LED6,0,0,0);
	set_rgb_led(LED8,0,0,0);
	set_front_led(0);
}

void attack_target(void){
	all_leds_red();
	move_str_dist(5, 1000);
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

void set_rgb_all_leds(int color){
	if(color == RED){
		set_rgb_led(LED2,10,0,0);
		set_rgb_led(LED4,10,0,0);
		set_rgb_led(LED6,10,0,0);
		set_rgb_led(LED8,10,0,0);
	}
	else if(color == BLUE){
		set_rgb_led(LED2,0,0,10);
		set_rgb_led(LED4,0,0,10);
		set_rgb_led(LED6,0,0,10);
		set_rgb_led(LED8,0,0,10);
	}
	else if(color == GREEN){
		set_rgb_led(LED2,0,10,0);
		set_rgb_led(LED4,0,10,0);
		set_rgb_led(LED6,0,10,0);
		set_rgb_led(LED8,0,10,0);
	}
	else{
		set_rgb_led(LED2,0,0,0);
		set_rgb_led(LED4,0,0,0);
		set_rgb_led(LED6,0,0,0);
		set_rgb_led(LED8,0,0,0);
	}
}


int front_dist_ir(void){
	int dist = (get_prox(0)+get_prox(7))/2;
	return dist;
}

void explore_maze(void){
	playMelody(MARIO_START, ML_SIMPLE_PLAY, NULL);
	calibration();

	int pos_tab = 0;
	int end_found = 0;
	chprintf((BaseSequentialStream *)&SD3, "Entrée parcours labyrinthe = %d \r\n", 1);
	left_motor_set_pos(0);

	while((pos_tab < size_tab)&&(!end_found)&&(!button_get_state())){
		int front_dist = front_dist_ir();

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
		}
		else if((get_main_color() == GREEN)||(get_main_color() == RED)||(get_main_color() == BLUE)){
			stopMotors();
			int couleur = get_main_color();
			chprintf((BaseSequentialStream *)&SD3, " couleur = %d \r", couleur);
			display_color_led();
			if (couleur == RED){
				attack_target();
				stopMotors();
			}
			else if(couleur == GREEN){
				stopMotors();
				set_body_led(1);
				playMelody(MARIO_FLAG, ML_SIMPLE_PLAY, NULL);
				chThdSleepMilliseconds(1000);
				end_found = 1;
				return;
			}
			stop_color_display();
		}
		else{
			stopMotors();

			tab_path[pos_tab] = left_motor_get_pos();
			int proxD = get_prox(2);
			int proxG = get_prox(5);
			if(proxD < 150){
				turn_right(300);
				tab_path[pos_tab+1] = 1;
				stopMotors();
			}
			else if(proxG < 150)
			{
				turn_left(300);
				tab_path[pos_tab] = 2;
				stopMotors();
			}
			else{
				u_turn(300);
				tab_path[pos_tab] = 3;
				stopMotors();
			}
			left_motor_set_pos(0);
			pos_tab += 2;
		}
		chThdSleepMilliseconds(100);
	}
	stopMotors();
}

void EP_call_home(){
	u_turn(500);
	chprintf((BaseSequentialStream *)&SD3, " Entrée tableau= %d \r\n", 1);
	for(int i = size_tab ; i > 0; i-=2){
		move_str_from_pos(tab_path[i-1], 1000);
		if(tab_path[i] == RIGHT)
		{
			turn_left(500);
			stopMotors();
		}
		else if(tab_path[i] == LEFT)
		{
			turn_right(500);
			stopMotors();
		}
		else if(tab_path[i] == UTURN){
			u_turn(500);
		}
		else{
			all_leds_red();
		}
		all_leds_off();
	}
	stopMotors();
	set_body_led(1);
	u_turn(500);
	show_panic(300, 3);
}

void calibration(void){
	int dist = VL53L0X_get_dist_mm();
	while(dist < 100){
		turn_right(300);
		chThdSleepMilliseconds(100);
		stopMotors();
		dist = VL53L0X_get_dist_mm();
	}
	chThdSleepMilliseconds(500);
	int diff = get_prox(6) - get_prox(1);
	while(diff > 150){
		turn_right(200);
		chThdSleepMilliseconds(50);
		stopMotors();
		diff = get_prox(6) - get_prox(1);
	}
	if(VL53L0X_get_dist_mm() < 100){
		calibration();
	}
	else{
		set_body_led(1);
		chThdSleepMilliseconds(500);
		set_body_led(0);
	}
}
void clean_tab(void){
	for(int i = 0; i<size_tab; i++){
		tab_path[i] = 0;
	}
}

void print_tab(void){
	for(int i = size_tab; i>0; i-=2){
		chprintf((BaseSequentialStream *)&SD3, "distance a faire = %d \r", tab_path[i-1]);
		chprintf((BaseSequentialStream *)&SD3, "prochain virage = %d \r\n", tab_path[i]);

	}
}

void ckeck_robot(void){
	chprintf((BaseSequentialStream *)&SD3, "Batterie=%d V = %f \r\n", get_battery_raw(), get_battery_voltage());
	chprintf((BaseSequentialStream *)&SD3, "Temperature ambiante = %d \r\n", get_temperature());
	chprintf((BaseSequentialStream *)&SD3, "Distance libre devant = %d\r\n", VL53L0X_get_dist_mm());
	chprintf((BaseSequentialStream *)&SD3, "Mic1 = %d Mic2 = %d Mic3 = %d Mic4 = %d \r\n",
			mic_get_volume(0), mic_get_volume(1), mic_get_volume(2), mic_get_volume(3));

	playNote(NOTE_AS4,100);

	set_rgb_all_leds(BLUE);
	chThdSleepMilliseconds(300);
	set_rgb_all_leds(GREEN);
	chThdSleepMilliseconds(300);
	set_rgb_all_leds(RED);
	set_rgb_all_leds(BLACK);
	set_front_led(0);
	set_body_led(0);
}

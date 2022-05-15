/*
 * usageMotors.c
 *
 *  Created on: 5 mai 2022
 *      Author: Louis&Valentin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <usageMotors.h>
#include <actionUser.h>
#include <motors.h>




#define PI                  3.1415926536f
#define WHEEL_DISTANCE      5.35f    //cm
#define PERIMETER_EPUCK     78*(PI * WHEEL_DISTANCE)

void go_straight(int speed){
	right_motor_set_speed(speed);
	left_motor_set_speed(speed);
}

void move_str_dist(int dist, int speed){
	int32_t position = left_motor_get_pos();
	int32_t cposition = 0;
	while (cposition < (position + dist*100)){
		cposition = left_motor_get_pos();
		left_motor_set_speed(speed);
		right_motor_set_speed(speed);
	}
	stopMotors();
}

void turn_right (int speed){
	right_motor_set_pos(0);
	left_motor_set_pos(0);
	while (left_motor_get_pos() < PERIMETER_EPUCK/4 && right_motor_get_pos() < PERIMETER_EPUCK/4){
		right_motor_set_speed(-speed);
		left_motor_set_speed(speed);
	}
}


void turn_left (int speed){
	right_motor_set_pos(0);
	left_motor_set_pos(0);
	while (left_motor_get_pos() < PERIMETER_EPUCK/4 && right_motor_get_pos() < PERIMETER_EPUCK/4){
		right_motor_set_speed(speed);
		left_motor_set_speed(-speed);
	}
}


void u_turn (int speed){
	right_motor_set_pos(0);
	left_motor_set_pos(0);
		while (left_motor_get_pos() < PERIMETER_EPUCK/2 && right_motor_get_pos() < PERIMETER_EPUCK/2){
			right_motor_set_speed(speed);
			left_motor_set_speed(-speed);
		}
}


void spin(int speed, int qty){
	int count = 0;
	while (count < qty){
		right_motor_set_pos(0);
		left_motor_set_pos(0);
		while (left_motor_get_pos() < PERIMETER_EPUCK && right_motor_get_pos() < PERIMETER_EPUCK){
			right_motor_set_speed(speed);
			left_motor_set_speed(-speed);
		}
		count += 1;
	}
}

void stopMotors(void){
	left_motor_set_speed(0);
	right_motor_set_speed(0);
	return;
}

void show_panic(int speed, int number){
	right_motor_set_pos(0);
	left_motor_set_pos(0);
	while (left_motor_get_pos() < PERIMETER_EPUCK/16 && right_motor_get_pos() < PERIMETER_EPUCK/16){
			right_motor_set_speed(speed);
			left_motor_set_speed(-speed);
	}
	for(uint8_t i=0; i<number; i++){
		right_motor_set_pos(0);
		left_motor_set_pos(0);
		while (left_motor_get_pos() < PERIMETER_EPUCK/8 && right_motor_get_pos() < PERIMETER_EPUCK/8){
			right_motor_set_speed(-speed);
			left_motor_set_speed(speed);
		}
		right_motor_set_pos(0);
		left_motor_set_pos(0);
		while (left_motor_get_pos() < PERIMETER_EPUCK/8 && right_motor_get_pos() < PERIMETER_EPUCK/8){
				right_motor_set_speed(speed);
				left_motor_set_speed(-speed);
		}

	}
	right_motor_set_pos(0);
	left_motor_set_pos(0);
	while (left_motor_get_pos() < PERIMETER_EPUCK/16 && right_motor_get_pos() < PERIMETER_EPUCK/16){
		right_motor_set_speed(-speed);
		left_motor_set_speed(speed);
	}
	stopMotors();
	leds_think(number);
}


void deviation_robot(int speedD, int speedG){
	left_motor_set_speed(speedG);
	right_motor_set_speed(speedD);
}


void move_str_from_pos(int position, int speed){
	left_motor_set_pos(0);
	int32_t cposition = 0;
	while (cposition < position){
		cposition = left_motor_get_pos();
		left_motor_set_speed(speed);
		right_motor_set_speed(speed);
	}
	stopMotors();
}



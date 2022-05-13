/*
 * usageMotors.h
 *
 *  Created on: 5 mai 2022
 *      Author: Louis&Valentin
 *
 *  Using the global library this module enables methods for an easier
 *  and wider usage of the robot's mouvements.
 *
 *  /!\ The spped used here goees from 0 to 1100
 */

#ifndef USAGEMOTORS_H_
#define USAGEMOTORS_H_

#define RIGHT 1
#define LEFT 2
#define UTURN 3

 /**
 * @brief   Sets the speed to @speed for both motors during an indefined time
 *
 * @param speed     speed desired in step/s
 */
void go_straight (int speed);

/**
* @brief   Sets the speed to @speed for both motors to advance of a certain distance
*
* @param 	dist	distance you want the robot to advance in cm
* 			speed	speed desired in step/s
*
*/
void move_str_dist(int dist, int speed);

/**
* @brief   Use this to make the robot turn right with a given speed.
*
* @param speed     speed desired in step/s
*/
void turn_right (int speed);

/**
* @brief   Use this to make the robot turn left with a given speed.
*
* @param speed     speed desired in step/s
*/
void turn_left (int speed);

/**
* @brief   Use this to make the robot do a U-turn with a given speed.
*
* @param speed     speed desired in step/s
*/
void u_turn (int speed);

/**
* @brief   Use this to make the robot do a given number full turn with a given speed.
*
* @param	speed    	speed desired in step/s
* 			qty			number of full turns
*/
void spin(int speed, int qty);

/**
* @brief   Stop the robot's motors.
*
*/
void stopMotors(void);

void show_panic(int speed, int number);

void turn_angle(int angle, int speed);

void deviation_robot(int speedD, int speedG);

void move_str_from_pos(int position, int speed);


#endif /* USAGEMOTORS_H_ */

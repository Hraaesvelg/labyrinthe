/*
 * actionUser.h
 *
 *  Created on: 5 mai 2022
 *      Author: Louis&Valentin
 */

#ifndef ACTIONUSER_H_
#define ACTIONUSER_H_


/**
 * @brief   Make the leds blink for a given number of time
 *
 * @param number     Number of time the leds will blink
 */
void blink_all_leds(int number);

/**
 * @brief   Led animation for thinking situations
 *
 * @param number     Number of time the animation will be played
 */
void leds_think(int number);

/**
 * @brief   Led animation for when the robot is blocked
 *
 * @param time     Time the animation will be played
 */
void leds_stop(int time);

/**
 * @brief   Make the top leds blink
 *
 * @param time     Time the animation will be played
 */
void up_leds_blink(int time);

/**
 * @brief   Turn on the 8 led of the top in red
 *
 */
void all_leds_red(void);

/**
 * @brief   Turn off the 8 leds of the top
 *
 */
void all_leds_off(void);

/**
 * @brief   Protocol executed when a target is detected
 *
 */
void attack_target(void);

/**
 * @brief   Set the 4 RGB leds to a desired color
 *
 * @param color     Color wanted for the 4 RGB leds (RED, GREEN, BLUE)
 */
void set_rgb_all_leds(int color);

/**
 * @brief   Main function of the project. Make the Robot move in the maze while avoiding obstacle.
 * Handle small action such as attacking enemies and detecting the end of the maze.
 *
 */
void explore_maze(void);

/**
 * @brief   Small animation to test a few leds (top, body, front)
 *
 * @param ite	Number d'iterations of the animation
 */
void initialisationLeds(int ite);

/**
 * @brief   Compute the mean distance to an obstacle in front of the robot using the ir-sensors
 *
 * @return  	A value corresponding to the distance (3500 = contact to captor / 50 = max mesurable distance
 *
 */

int front_dist_ir(void);

/**
*
* @brief   For a random start position the robot will rotate to be in the same orientation than the path
*
*/
void calibration(void);

/**
*
* @brief   Print in the uart terminal some infos about the robot
*
*/
void ckeck_robot(void);

#endif /* ACTIONUSER_H_ */


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

#endif /* ACTIONUSER_H_ */


/*
 * color.h
 *
 *  Created on: 9 mai 2022
 *      Author: Louis&Valentin
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "ch.h"
//#include "hal.h"

#define RED 1
#define BLUE 2
#define GREEN 3
#define BLACK 0
#define WHITE 4

// A few offset we can change to improve the mesures of colors
// As we only detect vivid color these offset aren't usefull.
//But they can be in other situations, that is why, we have decided to keep them/
static const int r_offset = 0; //9;
static const int g_offset = 0; //22;
static const int b_offset = 0; //19;
// Margin to avoid noise problems
static const int color_margin = 10;

/**
* @brief    Make a mean with all values in the buffer.
* 			/!\ this function only take the value in the middle of the buffer to avoid imprecisions.
*
* @param buffer 	pointer to the buffer with the pixels values
*
*/
uint8_t color_line(uint8_t *buffer);

/**
* @brief    Return the mean value of red on the line taken by the camera
*
* @return 	mean value of red on the image
*
*/
int get_red(void);

/**
* @brief    Return the mean value of blue on the line taken by the camera
*
* @return 	mean value of blue on the image
*
*/
int get_blue(void);

/**
* @brief    Return the mean value of green on the line taken by the camera
*
* @return 	mean value of green on the image
*
*/
int get_green(void);

/**
* @brief    Return the position of the line take in the field of view of the camera
*
* @return 	position of the line
*
*/
uint16_t get_line_position(void);

/**
* @brief    Start the thread to capture image with the camera to compute the main color
* 			That thread fill in the buffer with pixels values.
*
* @return 	position of the line
*
*/
void process_image_start(void);

/**
* @brief    Compare the mean values of each colors show the most dominant one in the picture
*
* @return 	Color RED, BLUE, GREEN
*
*/
int get_main_color(void);

/**
* @brief    Display the most dominant color on the rgb led by setting them the same color than the object in front of the robot
*
*/
void display_color_led(void);

/**
* @brief    Turn off the rgb leds
*
*/
void stop_color_display(void);

#endif /* COLOR_H_ */

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

uint8_t color_line(uint8_t *buffer);

int get_color_red(void);
int get_color_blue(void);
int get_color_green(void);

uint16_t get_line_position(void);

void process_image_start(void);



/*#define BLACK 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define COLOR_MARGIN 2000


void color_start(void);
uint8_t get_color_from_col(void);*/

#endif /* COLOR_H_ */

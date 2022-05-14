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

static const int r_offset = 0; //9;
static const int g_offset = 0; //22;
static const int b_offset = 0; //19;
static const int color_margin = 10;

uint8_t color_line(uint8_t *buffer);

int get_red(void);
int get_blue(void);
int get_green(void);

uint16_t get_line_position(void);

void process_image_start(void);

int get_main_color(void);

void display_color_led(void);

void stop_color_display(void);

#endif /* COLOR_H_ */

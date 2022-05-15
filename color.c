#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <main.h>
#include <camera/dcmi_camera.h>
#include <camera/po8030.h>
#include <leds.h>
#include <spi_comm.h>
#include <actionUser.h>
#include <color.h>

#define IMAGE_BUFFER_SIZE 640

static int red, blue, green  = 111;
static uint16_t line_position = IMAGE_BUFFER_SIZE/2; //middle

//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

/*
 *  Returns the line's width extracted from the image buffer given
 *  Returns 0 if line not found
 */

uint8_t color_line(uint8_t *buffer){
	int mean_color = 0;
	for(uint16_t i =  (IMAGE_BUFFER_SIZE)/4 ; i < (IMAGE_BUFFER_SIZE)*3/4  ; i++){
		mean_color += buffer[i];
	}
	mean_color /= IMAGE_BUFFER_SIZE/2;
	return mean_color;
}


static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
po8030_advanced_config(FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
po8030_set_awb(0); //pas blanc
dcmi_enable_double_buffering();
dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
dcmi_prepare();

    while(1){
        //starts a capture
dcmi_capture_start();
//waits for the capture to be done
wait_image_ready();
//signals an image has been captured
chBSemSignal(&image_ready_sem);

    }
}

static THD_WORKING_AREA(waProcessImage, 4096);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

uint8_t *img_buff;
static uint8_t image_red[IMAGE_BUFFER_SIZE], image_blue[IMAGE_BUFFER_SIZE], image_green[IMAGE_BUFFER_SIZE] = {0};

    while(1){
    //waits until an image has been captured
        chBSemWait(&image_ready_sem);
        img_buff  = dcmi_get_last_image_ptr();

		for(uint16_t i = 0 ; i < 2*(IMAGE_BUFFER_SIZE) ; i+=2){
			image_red[i/2] = ((uint8_t)img_buff[i]&0xF8)>> 3;
			image_blue[i/2] = (((uint8_t)img_buff[i+1]&0x1F));
			image_green[i/2] = ((((uint8_t)img_buff[i]&0x07 )<< 3)) | (((uint8_t)img_buff[i+1]&0xE0)>> 5);
		}
		red = color_line(image_red);
		blue = color_line(image_blue);
		green = color_line(image_green);
    }
}

int get_red(void){
	return red;
}
int get_blue(void){
	return blue;
}
int get_green(void){
	return green;
}

uint16_t get_line_position(void){
	return line_position;
}

void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO + 1, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO + 1, CaptureImage, NULL);
}

int get_main_color(void){
	int R, G, B = 0;
	R = get_red() - r_offset;
	G = get_green() - g_offset;
	B = get_blue() - b_offset;

	if(((R < color_margin) && (R > -color_margin))&&((B < color_margin) && (B > -color_margin))
			&&((G < color_margin) && (G > -color_margin))){
		return WHITE;
	}
	else if((R > B + color_margin)&&(R > G + color_margin)){
		return RED;
	}
	else if((B > R + color_margin)&&(B > G + color_margin)){
		return BLUE;
	}
	else if((G > R + color_margin)&&(G > B + color_margin)){
		return GREEN;
	}
	else{
		return BLACK;
	}
}

void display_color_led(void){
	int color = get_main_color();
	if(color == WHITE){
		set_rgb_led(0,10,10,10);
	}
	else if(color == RED){
		set_rgb_all_leds(RED);
	}
	else if(color == BLUE){
		set_rgb_all_leds(BLUE);
	}
	else if(color == GREEN){
		set_rgb_all_leds(GREEN);
	}
	else
	{
		set_rgb_all_leds(BLACK);
	}
}

void stop_color_display(void){
	set_rgb_all_leds(BLACK);
}

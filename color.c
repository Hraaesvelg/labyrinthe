#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <main.h>
#include <camera/po8030.h>
#include <camera/dcmi_camera.h>

#include <color.h>

#define IMAGE_BUFFER_SIZE 240

static int red, blue, green  = 111;
static uint16_t line_position = IMAGE_BUFFER_SIZE/2; //middle

//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

/*
 *  Returns the line's width extracted from the image buffer given
 *  Returns 0 if line not found
 */

uint8_t color_line(uint8_t *buffer){

uint32_t mean_color = 0;

for(uint16_t i =  (IMAGE_BUFFER_SIZE)/4 ; i < (IMAGE_BUFFER_SIZE)*3/4  ; i++){
mean_color += buffer[i];
}
mean_color /= IMAGE_BUFFER_SIZE;

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

static THD_WORKING_AREA(waProcessImage, 2048);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

uint8_t *img_buff;
uint8_t image_red[IMAGE_BUFFER_SIZE], image_blue[IMAGE_BUFFER_SIZE], image_green[IMAGE_BUFFER_SIZE] = {0};
    while(1){
    //waits until an image has been captured
        chBSemWait(&image_ready_sem);
        img_buff  = dcmi_get_last_image_ptr();

		for(uint16_t i = 0 ; i < 2*(IMAGE_BUFFER_SIZE) ; i+=2){
			chprintf((BaseSequentialStream *)&SD3, "R = %d \r\n", img_buff[i]);
			//extracts first 5bits of the first byte
			//takes nothing from the second byte
			image_red[i/2] = (uint8_t)img_buff[i]&0xF8;
			image_blue[i/2] = (((uint8_t)img_buff[i+1]&0x1F) <<3 );
			image_green[i/2] = (((uint8_t)img_buff[i]&0x07)<< 5) | (((uint8_t)img_buff[i+1]&0xE0)>> 3);
		}

		red = color_line(image_red);
		blue = color_line(image_blue); //color
		green = color_line(image_green);
    }
}

int get_color_red(void){
	return red;
}
int get_color_blue(void){
	return blue;
}
int get_color_green(void){
	return green;
}

uint16_t get_line_position(void){
	return line_position;
}

void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}








/*#include "ch.h"
//#include "hal.h"
#include <main.h>
#include <camera/po8030.h>
#include <camera/dcmi_camera.h>
#include <chprintf.h>
#include <color.h>

#define NB_POINTS		300

static uint8_t color = BLACK;

//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes 1/4 pixels at RGB565, in a band of 20 by 240 pixels
    po8030_advanced_config(FORMAT_RGB565,310,0, 20,240, SUBSAMPLING_X4, SUBSAMPLING_X4);
    //disable auto white balance adjustment
    po8030_set_awb(0);
    //change gains for every color to have equal measurement with white light
    po8030_set_rgb_gain(0x4A, 0x45, 0x5D);

    //preparations to take one picture
	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
	dcmi_prepare();
    while(TRUE){
        //starts a capture
		dcmi_capture_start();
		//waits for the capture to be done
		wait_image_ready();
		//signals an image has been captured
		chBSemSignal(&image_ready_sem);
		chprintf((BaseSequentialStream *)&SD3, "Image ready =%d \r\n",1 );
    }
}


static THD_WORKING_AREA(waProcessImage, 2048);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	uint8_t *img_buff_ptr;
	uint8_t red[NB_POINTS] = {0};
	uint8_t green[NB_POINTS] = {0};
	uint8_t blue[NB_POINTS] = {0};

    while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565    
		img_buff_ptr = dcmi_get_last_image_ptr();
		chprintf((BaseSequentialStream *)&SD3, "image filled = %d \r\n",1);

		//Extracts pixels
		for(uint16_t i = 0 ; i < (2 * NB_POINTS) ; i+=2){
			//5 first bits (>>3)*2,makes RGB values comparable on 6 bits
			red[i/2] = (img_buff_ptr[i]&0b11111000);
			//3 bits on the first ptr byte and 3 on the second one
			green[i/2] = ((img_buff_ptr[i]&0b00000111)<<5)+(((img_buff_ptr[i+1]&0b11100000)>>5));
			//5 last bits
			blue[i/2] = (img_buff_ptr[i+1]&0b00011111) <<3;
		}
		//sum on every pixels, better than mean
		uint32_t sum_red = 0;
		uint32_t sum_green = 0;
		uint32_t sum_blue = 0;
		for(uint16_t i = 0 ; i < (NB_POINTS) ; i++){
				sum_red += red[i];
				sum_green += green[i];
				sum_blue +=  blue[i];
		}
		chprintf((BaseSequentialStream *)&SD3, "R = %d B = %d V = %d \r\n",
								sum_red, sum_green, sum_blue );
		//identification of the most significant color, with an COLOR_MARGIN margin
		if((sum_red>(sum_blue+COLOR_MARGIN))&&((sum_green+COLOR_MARGIN)<sum_red)){
			color = RED;
		}
		else if(((sum_red+COLOR_MARGIN)<sum_green)&&(sum_green>(sum_blue+COLOR_MARGIN))){
			color = GREEN;
		}
		else if(((sum_red+COLOR_MARGIN)<sum_blue)&&((sum_green+COLOR_MARGIN)<sum_blue)){
			color = BLUE;
		}
		else{
			color = BLACK;
		}
    }
}

void color_start(void){
	chprintf((BaseSequentialStream *)&SD3, "waProcessImage = %d \r\n", color);
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chprintf((BaseSequentialStream *)&SD3, "waCaptureImage = %d \r\n", color);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}

uint8_t get_color_from_col(void){
	return color;
}

*/


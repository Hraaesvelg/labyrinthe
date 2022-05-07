#include <trouver_couleur.h>
#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <leds.h>
#include <main.h>
#include <camera/po8030.h>

#define IMAGE_BUFFER_SIZE		640


static float distance_cm = 0;
static uint16_t line_position = IMAGE_BUFFER_SIZE/2;	//middle


//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

/*uint16_t extract_line_width(uint8_t *buffer){

	uint16_t i = 0, begin = 0, end = 0, width = 0;
	uint8_t stop = 0, wrong_line = 0, line_not_found = 0;
	uint32_t mean = 0;

	static uint16_t last_width = PXTOCM/GOAL_DISTANCE;

	//performs an average
	for(uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){
		mean += buffer[i];
	}
	mean /= IMAGE_BUFFER_SIZE;

	do{
		wrong_line = 0;
		//search for a begin
		while(stop == 0 && i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE)){
			//the slope must at least be WIDTH_SLOPE wide and is compared
		    //to the mean of the image
		    if(buffer[i] > mean && buffer[i+WIDTH_SLOPE] < mean){
		        begin = i;
		        stop = 1;
		    }
		    i++;
		}
		//if a begin was found, search for an end
		if (i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE) && begin){
		    stop = 0;

		    while(stop == 0 && i < IMAGE_BUFFER_SIZE){
		        if(buffer[i] > mean && buffer[i-WIDTH_SLOPE] < mean){
		            end = i;
		            stop = 1;
		        }
		        i++;
		    }
		    //if an end was not found
		    if (i > IMAGE_BUFFER_SIZE || !end){
		        line_not_found = 1;
		    }
		}
		else if no begin was found {
		    line_not_found = 1;
		}

		//if a line too small has been detected, continues the search
		if(!line_not_found && (end-begin) < MIN_LINE_WIDTH){
			i = end;
			begin = 0;
			end = 0;
			stop = 0;
			wrong_line = 1;
		}
	}while(wrong_line);

	if(line_not_found){
		begin = 0;
		end = 0;
		width = last_width;
	}
	else {
		last_width = width = (end - begin);
		line_position = (begin + end)/2; //gives the line position.
	}

	//sets a maximum width or returns the measured width
	if((PXTOCM/width) > MAX_DISTANCE){
		return PXTOCM/MAX_DISTANCE;
	}
	else {
		return width;
	}
}*/


static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
	po8030_advanced_config(PO8030_FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1); //
	po8030_set_awb(0); // pas les blancs
	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(0);
	dcmi_prepare();

	systime_t time;
	time = chVTGetSystemTime();

    while(1){
        //starts a capture
		dcmi_capture_start();
		//waits for the capture to be done
		wait_image_ready();
		//signals an image has been captured
		chBSemSignal(&image_ready_sem);
    }
}


static THD_WORKING_AREA(waProcessImage, 1024);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	uint8_t *img_buff_ptr;
	uint8_t image_rouge[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image_verte[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image_bleue[IMAGE_BUFFER_SIZE] = {0};
	uint64_t rouge = 0;
	uint64_t vert = 0;
	uint64_t bleu = 0;


	bool send_to_computer = true;

	while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565
		img_buff_ptr = cam_get_last_image_ptr();

		for (uint16_t i = 0; i < 2*IMAGE_BUFFER_SIZE; i+=2){ // extrait couleur rouge
			image_rouge[i/2] = (uint8_t)img_buff_ptr[i]&0b11111000 >> 2;
		}

		for (uint16_t i = 0; i < 2*IMAGE_BUFFER_SIZE; i+=2){ // extrait couleur verte
			image_verte[i/2] = ((uint8_t)img_buff_ptr[i]&0b00000111 << 3)+ ((uint8_t)img_buff_ptr[i+1]&0b11100000);
		}
		for (uint16_t i = 0; i < 2*IMAGE_BUFFER_SIZE; i+=2){ // extrait couleur bleue
			image_bleue[i/2] = (uint8_t)img_buff_ptr[i+1]&0b0001111 * 2;
		}

		for (uint64_t i=0; i < IMAGE_BUFFER_SIZE; i++){
			int somme = 0;
			rouge = rouge + somme;
		}
		for (uint64_t i=0; i < IMAGE_BUFFER_SIZE; i++){
			int somme = 0;
			vert = vert + somme;
		}
		for (uint64_t i=0; i < IMAGE_BUFFER_SIZE; i++){
			int somme = 0;
			bleu = bleu + somme;
		}

		if (((rouge - vert) > 0.5 * rouge )&&((rouge - bleu) > 0.5 * rouge)){
			// mur est rouge

			set_led(0, 1);
		}
		else if ((vert - rouge) > 0.5 * vert && (vert - bleu) > 0.5 * vert){
			// mur est vert
			set_led(1, 1);
		}
		else if ((bleu - rouge) > 0.5 * bleu && (bleu - vert) > 0.5 * bleu){
			// mur est bleu
			set_led(2, 1);
		}
		else{
			//mur blanc
			set_led(3, 1);
		}
		//sends the data buffer of the given size to the computer
		/*if(send_to_computer){
			SendUint8ToComputer(image, IMAGE_BUFFER_SIZE);
		}

		send_to_computer = !send_to_computer;*/
    }
}


uint16_t get_line_position(void){
	return line_position;
}


void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}


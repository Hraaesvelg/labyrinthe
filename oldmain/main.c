#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include <motors.h>
#include <leds.h>
#include <selector.h>
#include <sensors/proximity.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <msgbus/messagebus.h>
#include <usageMotors.h>
#include <actionUser.h>
#include <threadSelector.h>
#include <chprintf.h>
#include <sensors/imu.h>
#include <trouver_couleur.h>
#include <threadMotor.h>
#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <allthreads.h>
#include <main.h>

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


// Jeu de lumi�re avec les led rouge/body/led
void initialisationLeds(int ite){
	clear_leds();
	int8_t compt = 0;
	while(compt <= ite){
		for(int8_t i=0; i<=3; i++){
			set_led(i, 1);
			chThdSleepMilliseconds(200);
			clear_leds();
		}
		compt += 1;
	}
	compt = 0;
	while(compt <= ite){
		set_body_led(1);
		set_front_led(0);
		chThdSleepMilliseconds(200);
		set_body_led(0);
		set_front_led(1);
		chThdSleepMilliseconds(200);
		compt +=1;
	}
	set_front_led(0);
	clear_leds();
}

void mouvementRdm(void){
	// calcul de la distance
	int distance = VL53L0X_get_dist_mm();
	chprintf((BaseSequentialStream *)&SD3, " Distance= %d \r\n",
			distance);

	if (distance > 50 ){
		go_straight(500);
	}
	else{
		stopMotors();
		int side = rand()%2;
		if (side==1){
			chThdSleepMilliseconds(50);
			turn_right(500);
		}
		else if (side == 0){
			chThdSleepMilliseconds(50);
			turn_left(500);
		}
	}
	chThdSleepMilliseconds(200);
}


int main(void)
{
	// Initialisation du systeme
    halInit();
    chSysInit();
    messagebus_init(&bus, &bus_lock, &bus_condvar);/** Inits the Inter Process Communication bus. */


    // Initialisation des modules compl�mentaires
    motors_init();
    serial_start();
    VL53L0X_start();
    imu_start();
    calibrate_gyro();
    cam_start();

    // fonction de debut
    initialisationLeds(1);
    //show_panic(700, 3);

    srand(2340);
    /* Boucle d'attente infinie */
    while (1) {
    	//get_image();
    	select_mode();
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}



/*int gyrox = get_gyro(0) - get_gyro_offset(0);
    	int gyroy = get_gyro(1) - get_gyro_offset(1);
    	//int gyrox = get_acc(0) - get_acc_offset(0);
    	//int gyroy = get_acc(1) - get_acc_offset(1);
    	uint8_t delta_gyro = 200;
    	chprintf((BaseSequentialStream *)&SD3, " GyroscopeX = %d GyroscopeY = %d \r\n",
    				gyrox, gyroy);
    	chThdSleepMilliseconds(200);

    	if(gyrox > delta_gyro){
    		clear_leds();
    		set_led(0, 1);
    	}
    	else if(gyrox < (-1)*delta_gyro){
    		clear_leds();
    		set_led(2, 1);
    	}
    	else if (gyroy > delta_gyro){
    		clear_leds();
    		set_led(1, 1);
    	}
    	else if (gyroy < (-1)*delta_gyro){
    		clear_leds();
    		set_led(3, 1);
    	}
    	else
    	{
    		//stopMotors();
    	}*/

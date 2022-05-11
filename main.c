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
#include <threadMotor.h>
#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <allthreads.h>
#include <main.h>
#include <camera/po8030.h>
#include <color.h>
#include <audio/microphone.h>
#include <usbcfg.h>
#include <audio/play_melody.h>
#include <spi_comm.h>


messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


static void serial_start(void)
{
    static SerialConfig ser_cfg = {
        115200,
        0,
        0,
        0,
    };

    sdStart(&SD3, &ser_cfg); // UART3. Connected to the second com port of the programmer
}

int main(void)
{
	/* System init */
    halInit();
    chSysInit();

    /** Inits the Inter Process Communication bus. */
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    spi_comm_start();
    // Initialisation des modules complémentaires
    motors_init();
    VL53L0X_start();
	serial_start();
	proximity_start();
	//playMelodyStart();
	dcmi_start();
	po8030_start();
	process_image_start();
    //thread perso à lancer pour le projet


    /* Boucle d'attente infinie */
    while (1) {
    	chprintf((BaseSequentialStream *)&SD3, "R = %d B = %d G = %d \r\n",
    	    			get_color_red(), get_color_green(), get_color_blue());

    	chThdSleepMilliseconds(150);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}


/*
chprintf((BaseSequentialStream *)&SD3, "IR1 = %d IR2 = %d IR3 = %d IR4 = %d IR5 = %d IR6 = %d IR7 = %d IR1 = %d \r\n",
    			get_prox(0), get_prox(1), get_prox(2), get_prox(3), get_prox(4),
				get_prox(5),get_prox(6), get_prox(7));

    	int front_dist = VL53L0X_get_dist_mm();
    	if(front_dist < 100){
    		int proxD = get_prox(2);
			int proxG = get_prox(5);
    		if((proxD < 100) | (proxG < 100)){
    			if((proxD < 100) & (proxG < 100)){
    				go_straight(500);
    			}
    			else if(proxD < 100){
    				stopMotors();
    				move_str_dist(5, 300);
    				turn_right(300);
    				stopMotors();
    			}
    			else
    			{
    				stopMotors();
					move_str_dist(5, 300);
					turn_left(300);
					stopMotors();
    			}
    		}
    		else{
    			go_straight(500);
    		}
    	}
    	else{
    		stopMotors();
    		int proxD = get_prox(2);
			int proxG = get_prox(5);
    		if(proxD < 100){
				move_str_dist(5, 300);
				turn_right(300);
				stopMotors();
			}
    		else if(proxD < 100)
			{
				move_str_dist(5, 300);
				turn_left(300);
				stopMotors();
			}
    		else{
				u_turn(300);
				stopMotors();
    		}

    		int front_dist = VL53L0X_get_dist_mm();
		if(front_dist < 100){
			stopMotors();
			int proxD = get_prox(2);
			int proxG = get_prox(5);
			if(proxD <= 150){
				stopMotors();
				turn_right(500);
				stopMotors();
			}
			else if(proxG <= 150){
				stopMotors();
				turn_left(500);
				stopMotors();
			}
			else{
				stopMotors();
				u_turn(500);
				stopMotors();
			}
		}
		else{
			go_straight(500);
			int proxD = get_prox(2);
			int proxG = get_prox(5);
			chprintf((BaseSequentialStream *)&SD3, "DROITE = %d GAUCHE = %d IR3\r\n",
			    			proxD, proxG);
			if((proxD <= 150)&(proxG > 150)){
				stopMotors();
				move_str_dist(5,300);
				stopMotors();
				turn_right(500);
				stopMotors();
			}
		}

 */



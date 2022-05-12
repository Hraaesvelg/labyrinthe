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
    	chprintf((BaseSequentialStream *)&SD3, "COULEUR = %d \r\n", get_main_color());
    	set_rgb_led(0,0,0,30);
		int front_dist = VL53L0X_get_dist_mm();
		chprintf((BaseSequentialStream *)&SD3, "DISTANCE = %d \r\n", front_dist);
		int proxD = get_prox(2);
		int proxG = get_prox(5);
		chprintf((BaseSequentialStream *)&SD3, "IR3 = %d IR6 = %d\r\n", proxD, proxG);

		if(front_dist > 100){
			int proxD = get_prox(2);
			int proxG = get_prox(5);
			go_straight(500);
			if((proxD < 150)&&(proxG > 130)){
				chprintf((BaseSequentialStream *)&SD3, "LIBRE A DROITE = %d \r\n", proxD, proxG);
				stopMotors();
				chThdSleepMilliseconds(500);
				move_str_dist(2,300);
				stopMotors();
				turn_right(300);
				stopMotors();
			}
		}
		else{
			stopMotors();
			int proxD = get_prox(2);
			int proxG = get_prox(5);
			if(proxD < 150){
				move_str_dist(5, 300);
				turn_right(300);
				stopMotors();
			}
			else if(proxG < 150)
			{
				move_str_dist(5, 300);
				turn_left(300);
				stopMotors();
			}
			else{
				u_turn(300);
				stopMotors();
			}
		}
		chThdSleepMilliseconds(100);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}



/*
 * if(front_dist < 100){
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
		}
 */

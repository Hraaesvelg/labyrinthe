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
#include <chprintf.h>
#include <sensors/imu.h>
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
#include "button.h"

#include "audio/audio_thread.h"
#include "audio/play_melody.h"
#include "audio/play_sound_file.h"
#include "audio/microphone.h"

#include "audio/audio_thread.h"
#include "audio/play_melody.h"
#include "audio/play_sound_file.h"
#include "audio/microphone.h"
#include "sdio.h"

#include <sensors/battery_level.h>


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
	dcmi_start();
	po8030_start();
	process_image_start();
	battery_level_start();

	// initialisation du son pour les musique
	dac_start();
	mic_start(NULL);
	playMelodyStart();
	playSoundFileStart();

    //thread perso à lancer pour le projet
	main_thread_start();

    while (1) {
		chThdSleepMilliseconds(100);
    }
}


#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}



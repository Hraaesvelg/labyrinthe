/*
 * threadSelector.c
 *
 *  Created on: 7 mai 2022
 *      Author: Louis&Valentin
 */


#include <selector.h>
#include <allthreads.h>
#include <usageMotors.h>
#include <actionUser.h>
#include <trouver_couleur.h>
#include "ch.h"
#include "hal.h"
#include "memory_protection.h"


void select_mode(void){
	// Capter la valeur du selecteur
	int selector = get_selector();
	switch (selector){
				case 0: // Labyrinthe
					thread_motor();
					break;
				case 1:
					get_image();
					break;
				case 2:

					break;
				case 3:

					break;
				default:
					up_leds_blink(200);
					break;
			}
	    	stopMotors();
}



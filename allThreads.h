/*
 * allThreads.h
 *
 *  Created on: 7 mai 2022
 *      Author: Louis&Valentin
 */

#ifndef ALLTHREADS_H_
#define ALLTHREADS_H_

#include "ch.h"

#define ACTIVE 1
#define NONACTIVE 0

void thread_motor(void);
void enable_thread_motor(void);
void disable_thread_motor(void);
uint8_t get_running_motor(void);


#endif /* ALLTHREADS_H_ */

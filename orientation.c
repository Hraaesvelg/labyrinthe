#include <sensors/proximity.h>
#include <usageMotors.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <motors.h>
#include <orientation.h>

void good_inclinaison (void){
	int distance = VL53L0X_get_dist_mm();
	if (distance < 100){
		turn_left(MOTOR_SPEED_LIMIT/3);
		chThdSleepMilliseconds(50);
		int dist2 = VL53L0X_get_dist_mm();
		if (dist2 < 100){
			turn_left(MOTOR_SPEED_LIMIT/3);
			chThdSleepMilliseconds(50);
			int dist3 = VL53L0X_get_dist_mm();
			if (dist3 < 100){
				turn_left(MOTOR_SPEED_LIMIT/3);
			}
		}
	}
	else {
		int diff1 = get_prox(0) - get_prox(6);
		int diff2 = get_prox(1) - get_prox(7);
		if (diff1 > 40 && diff2 > 40){
			while (diff1 > 40 && diff2 > 40){
				diff1 = get_prox(0) - get_prox(6);
				diff2 = get_prox(1) - get_prox(7);
				right_motor_set_speed(-MOTOR_SPEED_LIMIT/10);
				left_motor_set_speed(MOTOR_SPEED_LIMIT/10);
			}
		}
		else if (diff1 < -40 && diff2 < -40){
			while (diff1 < -40 && diff2 < -40){
				diff1 = get_prox(0) - get_prox(6);
				diff2 = get_prox(1) - get_prox(7);
				right_motor_set_speed(MOTOR_SPEED_LIMIT/10);
				left_motor_set_speed(-MOTOR_SPEED_LIMIT/10);
			}
		}

	}
}


void calibration(void){
	int dist = VL53L0X_get_dist_mm();
	while(dist < 100){
		turn_right(300);
		chThdSleepMilliseconds(100);
		stopMotors();
		dist = VL53L0X_get_dist_mm();
	}
	chThdSleepMilliseconds(500);
	int diff = get_prox(6) - get_prox(1);
	while(diff > 150){
		turn_right(200);
		chThdSleepMilliseconds(50);
		stopMotors();
		diff = get_prox(6) - get_prox(1);
	}
}


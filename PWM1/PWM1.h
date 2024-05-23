/*
 * PWM1.h
 *
 * Created: 19/05/2024 11:13:16
 *  Author: pablo
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#include <stdint.h>
#include <avr/io.h>

void init_PWM1_FAST(void);
void updateDutyCicle1A(uint16_t duty);
void updateDutyCicle1B(uint16_t duty);



#endif /* PWM1_H_ */
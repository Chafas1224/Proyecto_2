/*
 * PWM2.h
 *
 * Created: 19/05/2024 11:27:42
 *  Author: pablo
 */ 


#ifndef PWM2_H_
#define PWM2_H_

#include <stdint.h>
#include <avr/io.h>

void init_PWM2_FAST(void);
void updateDutyCicle2A(uint8_t duty);
void updateDutyCicle2B(uint8_t duty);

#endif /* PWM2_H_ */
/*
 * PWM2.c
 *
 * Created: 19/05/2024 11:30:03
 *  Author: pablo
 */ 

#include "PWM2.h"

void init_PWM2_FAST(void){
	DDRB |= (1 << DDB3);
	DDRD |= (1 << DDD3);
	
	TCCR2A = 0;
	TCCR2B = 0;
	
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<COM2B1);
	
	TCCR2A |= (1<<WGM21)|(1<<WGM20);
	
	//Prescaler de 256
	TCCR2B |= (1<<CS22)|(1<<CS21);
}

void updateDutyCicle2A(uint8_t duty){
	OCR2A = duty;
}

void updateDutyCicle2B(uint8_t duty){
	OCR2B = duty;
}
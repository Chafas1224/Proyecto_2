/*
 * PWM1.c
 *
 * Created: 19/05/2024 11:16:10
 *  Author: pablo
 */ 

#include "PWM1.h"

void init_PWM1_FAST(void){
	DDRB |= (1 << DDB1) | (1 << DDB2);
	
	TCCR1A = 0;
	TCCR1B = 0;
	
	TCCR1A |= (1 << COM1A1);
	TCCR1A |= (1 << COM1B1);  
	
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12)|(1<<WGM13);
	
	//Prescaler de 64
	TCCR1B |= (1<<CS11)|(1<<CS10);
	
	//TOP de 1249
	ICR1H = 0x04;
	ICR1L = 0xE1;
}

void updateDutyCicle1A(uint16_t duty){
// 	OCR1AH = (duty & 0xFF00) >> 8;
// 	OCR1AL = (duty & 0x00FF);
	OCR1A = duty;
}

void updateDutyCicle1B(uint16_t duty){
// 	OCR1BH = (duty & 0xFF00) >> 8;
// 	OCR1BL = (duty & 0x00FF);
	OCR1B = duty;
}
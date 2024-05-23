/*
 * ADC.c
 *
 * Created: 19/05/2024 11:42:39
 *  Author: pablo
 */ 

#include "ADC.h"

void initADC(void){
	ADMUX = 0;
	//Voltaje de referencia = 5V
	ADMUX |= (1<<REFS0);
	//justificado a la izquierda
	ADMUX |= (1<<ADLAR);
	ADCSRA = 0;
	//Habilitar ADC
	ADCSRA |= (1<<ADEN);
	//Máscara ADC
	ADCSRA |= (1<<ADIE);
	//Prescaler a 128
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
	ADCSRB = 0;
	
	DIDR0 |= (1<<ADC0D) | (1<<ADC1D) | (1<<ADC2D) | (1<<ADC3D);
	
	
	//Iniciar primera conversión
	ADCSRA |= (1<<ADSC);
}
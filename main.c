/*
 * Proyecto__2.c
 *
 * Created: 19/05/2024 11:12:38
 * Author : pablo
 */ 

#define F_CPU 4000000
#include <avr/io.h>
#include "ADC/ADC.h"
#include "PWM1/PWM1.h"
#include "PWM2/PWM2.h"
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/eeprom.h>

uint8_t valor_pot1 = 0;
uint16_t dutyCycle1 = 0;
uint8_t valor_pot2 = 0;
uint8_t dutyCycle2 = 0;
uint8_t valor_pot3 = 0;
uint8_t dutyCycle3 = 0;
uint8_t valor_pot4 = 0;
uint8_t dutyCycle4 = 0;
uint8_t alternador_pots = 0;
uint8_t modo = 0;
uint8_t memoria = 1;
uint8_t valor_eeprom = 0;


void setup(void);

//----------------------------MAPEOS-------------------------------------------------
uint16_t mapADCtoServo1(uint8_t valor_ADC1){
	return (uint16_t)(((float)valor_ADC1/255)*(149.88-37.47)+37.47);
}

uint16_t mapADCtoServo2(uint8_t valor_ADC2){
	return (uint16_t)(((float)valor_ADC2/255)*(149.88-37.47)+37.47);
}

uint8_t mapADCtoServo3(uint8_t valor_ADC3){
	return (uint8_t)(((float)valor_ADC3/255)*(31-5)+5);
}

uint8_t mapADCtoServo4(uint8_t valor_ADC4){
	return (uint8_t)(((float)valor_ADC4/255)*(31-5)+5);
}
//----------------------------------------------------------------------------------

//----------------------------FUNCIONES-------------------------------------------------
void config_pines(void){
	//Botones:
	//Cambio de modo
	DDRC &= ~(1 << DDC2);
	PORTB |= (1 << PORTC2);
	
	//Cambiar la localidad
	DDRC &= ~(1 << DDC3);
	PORTB |= (1 << PORTC3);
	

	//Guardar la localidad
	DDRC &= ~(1 << DDC1);
	PORTC |= (1 << PORTC1);
	
	//LEDS
	DDRD |= (1 << DDD7) | (1 << DDD4) | (1 << DDD2) | (1 << DDD5) | (1 << DDD6);
	DDRB |= (1 << DDB4) | (1 << DDB5);
	
	//Interrupciones
	PCICR |= (1 << PCIE2) | (1 << PCIE1)| (1 << PCIE0);
	PCMSK1 |= (1 << PCINT9)	| (1 << PCINT10) | (1 << PCINT11); // PC0
	
}


void leds_memoria (void){
	if (memoria == 0)
	{
		PORTB |= (1 << PINB5);
		PORTD &= ~(1 << PIND6);
		PORTB &= ~(1 << PINB4);
	}
	else if (memoria == 1)
	{
		PORTD |= (1 << PIND6);
		/*PORTD &= ~(1 << PIND5);*/
		PORTB &= ~(1 << PINB4) & ~(1 << PIND5);
	}
	else if (memoria == 2)
	{
		PORTB |= (1 << PINB4);
		PORTD &= ~(1 << PIND6) /*/& ~(1 << PIND5)/*/;
		PORTB &= ~(1 << PINB5);
	}
	else if (memoria == 3)
	{
		PORTD |= (1 << PIND6)/*/| (1<<PIND5)/*/;
		PORTB |= (1 << PINB5);
		PORTB &= ~(1 << PINB4);
	}
}

//----------------------------------------------------------------------------------


//----------------------------MAIN-------------------------------------------------
int main(void)
{
    CLKPR = (1<<CLKPCE);
    CLKPR = (1<<CLKPS1); // CLK a 4 MHz
	setup();
	config_pines();
    while (1) 
    {
		ADCSRA |= (1 << ADSC);  // Iniciar conversión
		_delay_ms(1000);
		if (modo == 0)
		{
			PORTD |= (1 << PIND2);
			PORTD &= ~(1 << PIND4) & ~(1 << PIND7);
			dutyCycle1 = mapADCtoServo1(valor_pot1);
			updateDutyCicle1A(dutyCycle1);
			
			
			dutyCycle2 = mapADCtoServo2(valor_pot2);
			updateDutyCicle1B(dutyCycle2);
			
			
			dutyCycle3 = mapADCtoServo3(valor_pot3);
			updateDutyCicle2A(dutyCycle3);
			
			
			dutyCycle4 = mapADCtoServo4(valor_pot4);
			updateDutyCicle2B(dutyCycle4);
		}
		
		
    }
}
//----------------------------------------------------------------------------------



//----------------------------SETUP-------------------------------------------------
void setup(void){
	cli();
	DDRC = 0;
	init_PWM1_FAST();
	init_PWM2_FAST();
	//leds_memoria();
	
	initADC();
	sei();
}
//----------------------------------------------------------------------------------


//----------------------------INTERRUPCIONES-------------------------------------------------
ISR(ADC_vect){
	ADCSRA &= ~(1<<ADEN);
	if (alternador_pots == 0)
	{
		ADMUX = ((ADMUX & ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0))) | ((1<<MUX2)|(1<<MUX1)|(1<<MUX0)));
		valor_pot1 = ADCH;
		alternador_pots +=1;
	}
	else if (alternador_pots == 1)
	{
		ADMUX = ((ADMUX & ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0))) | ((1<<MUX2)|(1<<MUX1)));
		valor_pot2 = ADCH;
		alternador_pots +=1;
	}
	else if (alternador_pots == 2)
	{
		ADMUX = ((ADMUX & ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0))) | ((1<<MUX2)|(1<<MUX0)));
		valor_pot3 = ADCH;
		alternador_pots +=1;
	}
	else if (alternador_pots == 3)
	{
		ADMUX = ((ADMUX & ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0))) | (1<<MUX2));
		valor_pot4 = ADCH;
		alternador_pots = 0;
	}
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);
	ADCSRA |= (1<<ADIF);

}

ISR (PCINT1_vect){
	if (!(PINC & (1 << PINC3)))
	{
		_delay_ms(100);
		if (!(PINC & (1 << PINC3))) //BOTON 1, CAMBIO DE MODO
		{
			if (modo == 0) //MODO MANUAL, GUARDAR, CAMBIAR DE LOCACION
			{
				PORTD |= (1 << PIND2);
				PORTD &= ~(1 << PIND4) & ~(1 << PIND7);
				modo +=1;
			}
			else if (modo == 1) //MODO EEPROM, SUBIR VALORES A SERVOS
			{
				PORTD |= (1 << PIND7);
				PORTD &= ~(1 << PIND2) & ~(1 << PIND4);
				modo +=1;
			}
			else if (modo == 2)//ADAFRUIT, UART PRACTICAMENTE, MOVER SERVOS, GUARDAR, CAMBIAR LOCACION
			{
				PORTD |= (1 << PIND4);
				PORTD &= ~(1 << PIND2) & ~(1 << PIND7);
				modo +=1;
				/*/modo = 0;/*/
			}
			else if (modo ==3)//ADAFRUIT, SUBIR VALORES A SERVOS
			{
				PORTD |= (1 << PIND2) | (1 << PIND7);
				PORTD &= ~(1 << PIND4);
				modo = 0;
			}
			
		}
	}
	
	
	
	if (!(PINC & (1 << PINC2)))//BOTON 2, CAMBIA LA LOCALIDAD
	{
		_delay_ms(100);
		if (!(PINC & (1 << PINC2)))
		{
			// 		if ((modo == 0) | (modo == 1))
			// 		{
			if (memoria == 4)
			{
				memoria = 1;
				leds_memoria();
			}
			else{
				memoria++;
				leds_memoria();
			}
			/*/}/*/
		}
	}

	
	if (!(PINC & (1 << PINC1)))
	{
		if (modo == 0)
		{
			if (memoria == 1){
				eeprom_write_byte((uint8_t*)0, OCR1A);
				eeprom_write_byte((uint8_t*)1, OCR1B);
				eeprom_write_byte((uint8_t*)2, OCR2A);
				eeprom_write_byte((uint8_t*)3, OCR2B);
				_delay_ms(500);
				/*/leds_memoria();/*/
			}
			
			if (memoria == 2){
				eeprom_write_byte((uint8_t*)4, OCR1A);
				eeprom_write_byte((uint8_t*)5, OCR1B);
				eeprom_write_byte((uint8_t*)6, OCR2A);
				eeprom_write_byte((uint8_t*)7, OCR2B);
				_delay_ms(500);
				/*/leds_memoria();/*/
			}
			
			if (memoria == 3){
				eeprom_write_byte((uint8_t*)8, OCR1A);
				eeprom_write_byte((uint8_t*)9, OCR1B);
				eeprom_write_byte((uint8_t*)10, OCR2A);
				eeprom_write_byte((uint8_t*)11, OCR2B);
				_delay_ms(500);
				/*/leds_memoria();/*/
			}
			
			if (memoria == 4){
				eeprom_write_byte((uint8_t*)12, OCR1A);
				eeprom_write_byte((uint8_t*)13, OCR1B);
				eeprom_write_byte((uint8_t*)14, OCR2A);
				eeprom_write_byte((uint8_t*)15, OCR2B);
				_delay_ms(500);
				/*/leds_memoria();/*/
			}
		}
		else if (modo == 1){
			if (memoria == 1){
				valor_eeprom = eeprom_read_byte((uint8_t*)0);
				OCR1A = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)1);
				OCR1B = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)2);
				OCR2A = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)3);
				OCR2B = valor_eeprom;
				/*/leds_memoria();/*/
			}
			
			if (memoria == 2){
				valor_eeprom = eeprom_read_byte((uint8_t*)4);
				OCR1A = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)5);
				OCR1B = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)6);
				OCR2A = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)7);
				OCR2B = valor_eeprom;
				/*/leds_memoria();/*/
			}
			
			if (memoria == 3){
				valor_eeprom = eeprom_read_byte((uint8_t*)8);
				OCR1A = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)9);
				OCR1B = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)10);
				OCR2A = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)11);
				OCR2B = valor_eeprom;
				/*/leds_memoria();/*/
			}
			
			if (memoria == 4){
				valor_eeprom = eeprom_read_byte((uint8_t*)12);
				OCR1A = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)13);
				OCR1B = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)14);
				OCR2A = valor_eeprom;
				valor_eeprom = eeprom_read_byte((uint8_t*)15);
				OCR2B = valor_eeprom;
				/*/leds_memoria();/*/
			}
		}
		
	}
}
//----------------------------------------------------------------------------------
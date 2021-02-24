/*
 * main.c
 *
 * Created: 2/13/2021 7:55:59 PM
 *  Author: simon
 */ 

/*REG D LEDS and MAGNET

*/
#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define F_CPU 16000000UL
#define BASE_FREQ 80.0

ISR (TIMER0_COMPA_vect){
	PORTD |= (1 << PD2)|(1 << PD1)|(1 << PD0);
}

ISR	(TIMER0_COMPB_vect){
	PORTD &= ~((1 << PD2)|(1 << PD1)|(1 << PD0));
	TCNT0 = 0;
}

ISR (TIMER2_COMPA_vect){
	PORTD |= (1 << PD3);
}

ISR (TIMER2_COMPB_vect){
	PORTD &= ~(1 << PD3);
	TCNT2 = 0;
}



int main(void){
	cli();
	
	//CLKPR = (1 << CLKPCE)|(0 << CLKPS3)|(0 << CLKPS2)|(0 << CLKPS1)|(0 << CLKPS0);
	//CLKPR = (0 << CLKPS3)|(0 << CLKPS2)|(0 << CLKPS1)|(0 << CLKPS0);

	DDRD = 0xFF; //LEDs and Magnet output set as outputs
	
	float freq_magn = BASE_FREQ;
	float dutyc_magn = 1.0; //Percent
	long time_magn = round(F_CPU/1024/freq_magn);
	
	float freq_led = BASE_FREQ+0.5; //Plus freq shift
	float dutyc_led = 1;  //Percent
	float time_led = round((F_CPU/1024)/freq_led);

	OCR0A = round(time_led);
	OCR0B = round(time_led+dutyc_led*time_led); //Ändra så dutycycle ofrekvens blir rätt med summan
	TCCR0A = 0;
	TCCR0B = 0;
	TCCR0B = (1 << CS01)|(1 << CS00);		//Prescaler 1024
	TIMSK0 = (1 << OCIE0A)|(1 << OCIE0B);	//Cause interupt on flag
	

	OCR2A = round(time_magn);
	OCR2B = round(time_magn+dutyc_magn*time_magn);
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2B = (1 << CS22)|(1 << CS21);		//Prescaler 1024
	TIMSK2 = (1 << OCIE2B)|(1 << OCIE2A);	//Cause interupt on flag
	
	sei();
	
	while(1)
	{
		continue;
	}
}


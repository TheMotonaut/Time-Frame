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

#include <util/delay.h>

#define BASE_FREQ 68.0
#define FREQ_SHIFT 2.0
#define STROBE_CTRL 0x0F // Outputs for leds and magnet

void shift_freq2(void);
void shift_freq(void);
void led_init(void);
void magnet_init(void);
void rgb_init(void);

unsigned int program = 0;

ISR (TIMER0_COMPA_vect){
	PORTD |= (1 << PD2)|(1 << PD1)|(1 << PD0);
}

ISR	(TIMER0_COMPB_vect){
	PORTD &= ~((1 << PD2)|(1 << PD1)|(1 << PD0));
	TCNT0 = 0;
	TCNT1 = 0;
}

ISR (TIMER2_COMPA_vect){
	PORTD |= (1 << PD3);
}

ISR (TIMER2_COMPB_vect){
	PORTD &= ~(1 << PD3);
	TCNT2 = 0;
}

ISR (TIMER1_COMPB_vect){
	PORTD &= ~(1 << PD1);
}

ISR (TIMER1_COMPA_vect){
	PORTD &= ~(1 << PD2);
}




int main(void){
	cli();

	DDRD = STROBE_CTRL; // Set magnet and led pins as output
	DDRB = 0; //PortB as inputs
	
	PORTB |= (1 << PB0)|(1 << PB2); //Pull-up on PB0 and PB2
	
	led_init();
	magnet_init();
	rgb_init();

	sei();
	
	while(1);
	
}

void led_init(){
	float freq_led = BASE_FREQ + FREQ_SHIFT;				//Plus freq shift
	float dutyc_led = 0.2;						//Duty-cycle, scales brightness. Strobe effect removed on too high brightness
	float time_led = round((F_CPU/1024)/freq_led);

	OCR0A = round(time_led-dutyc_led*time_led);
	OCR0B = round(time_led);
	TCCR0A = 0;
	TCCR0B = 0;
	TCCR0B = (1 << CS01)|(1 << CS00);
	TIMSK0 = (1 << OCIE0A)|(1 << OCIE0B);		//Cause interupt on flag
}

void rgb_init(){
	float freq_led = BASE_FREQ + FREQ_SHIFT;
	float dutyc_led = 0.2;
	float rgb_cali1 = 0.8;
	float rgb_cali2 = 0.1;
	float time_led = round((F_CPU/1024)/freq_led);

	OCR1A = round(time_led - rgb_cali1 * dutyc_led * time_led);
	OCR1B = round(time_led - rgb_cali2 * dutyc_led * time_led);
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TCCR1B = (1 << CS11)|(1 << CS10);
	TIMSK1 = (1 << OCIE1A)|(1 << OCIE1B);
}

void magnet_init(){
	float freq_magn = BASE_FREQ;
	float dutyc_magn = 0.2;						//Duty-cycle
	long time_magn = round(F_CPU/1024/freq_magn);
	
	OCR2A = round(time_magn-dutyc_magn*time_magn);
	OCR2B = round(time_magn);
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2B = (1 << CS22);						
	TIMSK2 = (1 << OCIE2B)|(1 << OCIE2A);		//Cause interupt on flag
}


void shift_freq(){
	int i;
	for (i = 10; i < 50; i++){
		float freq_led = BASE_FREQ + i/10;				//Plus freq shift
		float dutyc_led = 0.05;						//Duty-cycle, scales brightness. Strobe effect removed on too high brightness
		float time_led = round((F_CPU/1024)/freq_led);
				
		OCR0A = round(time_led-dutyc_led*time_led);
		OCR0B = round(time_led);
		_delay_ms(10);
	}
	int j;
	for (j = 50; j > 10; j--){
		float freq_led = BASE_FREQ + j/10;				//Plus freq shift
		float dutyc_led = 0.05;						//Duty-cycle, scales brightness. Strobe effect removed on too high brightness
		float time_led = round((F_CPU/1024)/freq_led);
				
		OCR0A = round(time_led-dutyc_led*time_led);
		OCR0B = round(time_led);
		_delay_ms(10);
	}
} 

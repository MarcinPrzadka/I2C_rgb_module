#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


volatile uint8_t pwm1R, pwm1G, pwm1B, pwm2R, pwm2G, pwm2B;
#define F_CPU = 8000000;

int main(void){

	DDRB |= (1<<PB3);
	
	TCCR0 |= (1<<WGM01)|(1<<WGM00);
	TCCR0 |= (1<<COM01);
	TCCR0 |= (1<<CS00);
	OCR0 = 255;

	DDRD  |= (1<<PD7)|(1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3)|(1<<PD2);
	PORTD |= (1<<PD7)|(1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3)|(1<<PD2);

	TCCR2 |= (1<<WGM21);
	TCCR2 |= (1<<CS20);
	OCR2 = 200;
	TIMSK |= (1<<OCIE2);
	
	sei();
	uint8_t i;
	while(1){
		for(i = 0; i < 255; i++){
			OCR0 = i;
			pwm1R = i;
			pwm1G = i;
			pwm1B = i;
			pwm2R = i;
			pwm2G = i;
			pwm2B = i;
			_delay_ms(3);
		}
		_delay_ms(500);
	}
}

ISR( TIMER2_COMP_vect){
	static uint8_t cnt;
	if(cnt>= pwm1R) PORTD |= (1<<PD2); else PORTD &= ~(1<<PD2);
	if(cnt>= pwm1G) PORTD |= (1<<PD3); else PORTD &= ~(1<<PD3);
	if(cnt>= pwm1B) PORTD |= (1<<PD4); else PORTD &= ~(1<<PD4);
	if(cnt>= pwm2R) PORTD |= (1<<PD5); else PORTD &= ~(1<<PD5);
	if(cnt>= pwm2G) PORTD |= (1<<PD6); else PORTD &= ~(1<<PD6);
	if(cnt>= pwm2B) PORTD |= (1<<PD7); else PORTD &= ~(1<<PD7);
	cnt++;	
}

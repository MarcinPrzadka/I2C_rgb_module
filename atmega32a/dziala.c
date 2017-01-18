#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t pwm1R, pwm1G, pwm1B, pwm2R, pwm2G, pwm2B,pwm3R, pwm3G, pwm3B, pwm4R, pwm4G, pwm4B;

int main(void)
{
	
	DDRB |= (1<<PB3);

	TCCR0 |= (1<<WGM01)|(1<<WGM00);		// tryb Fast PWM
	TCCR0 |= (1<<COM01);				// clear at TOP
	TCCR0 |= (1<<CS00);					// preskaler = 1
	OCR0=255;
								// wygaszenie diody w kanale PWM
	DDRD |= (1<<PD0)|(1<<PD1)|(1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5);
	DDRC |= (1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)|(1<<PC6)|(1<<PC7);
	PORTD |= (1<<PD0)|(1<<PD1)|(1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5);
	PORTC |= (1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)|(1<<PC6)|(1<<PC7);

	TCCR2 |= (1<<WGM21);	// tryb  CTC
	TCCR2 |= (1<<CS20);		// preskaler = 1
	OCR2 = 199;				// dodatkowy podzia³ czêsttotliwoœci przez 200
	TIMSK |= (1<<OCIE2);	// zezwolenie na przerwanie CompareMatch
	sei();				// odblokowanie globalne przerwañ
	unsigned char data;
	TWI_init_slave();

	while(1)
	{
		//data = TWI_read_slave();
		//if(data == "255") pwm1G = 255;
		//Police();
		
		GrenLantern();
		
	}

}

// Funkcje swiecace

Police(){
		LedClean();
		pwm1R=255;
		pwm2B=255;
		pwm1B=0;
		pwm2R=0;
		_delay_ms(3);
		pwm1R=0;
		pwm2B=0;
		pwm1B=255;
		pwm2R=255;
		_delay_ms(3);
}
GreenLantern(){
	
	LedClean();
	pwm1G = 255;
	pwm2G = 255;
	pwm3G = 255;
	pwm4G = 255;

}

LedClean(){
	pwm1R = 0;
	pwm1B = 0;
	pwm1G = 0;
	pwm2R = 0;
	pwm2B = 0;
	pwm2G = 0;
	pwm3R = 0;
	pwm3B = 0;
	pwm3G = 0;
	pwm4R = 0;
	pwm4B = 0;
	pwm4G = 0;

}




ISR( TIMER2_COMP_vect ) // cia³o procedury obs³ugi przerwania Compare Match Timera2
{
	static uint8_t cnt; 
	if(cnt>=pwm1R) PORTD |= (1<<PD0); else PORTD &= ~(1<<PD0);
	if(cnt>=pwm1G) PORTD |= (1<<PD1); else PORTD &= ~(1<<PD1);
	if(cnt>=pwm1B) PORTD |= (1<<PD2); else PORTD &= ~(1<<PD2);
	if(cnt>=pwm2R) PORTD |= (1<<PD3); else PORTD &= ~(1<<PD3);
	if(cnt>=pwm2G) PORTD |= (1<<PD4); else PORTD &= ~(1<<PD4);
	if(cnt>=pwm2B) PORTD |= (1<<PD5); else PORTD &= ~(1<<PD5);
	if(cnt>=pwm3R) PORTC |= (1<<PC2); else PORTC &= ~(1<<PC2);
	if(cnt>=pwm3G) PORTC |= (1<<PC3); else PORTC &= ~(1<<PC3);
	if(cnt>=pwm3B) PORTC |= (1<<PC4); else PORTC &= ~(1<<PC4);
	if(cnt>=pwm4R) PORTC |= (1<<PC5); else PORTC &= ~(1<<PC5);
	if(cnt>=pwm4G) PORTC |= (1<<PC6); else PORTC &= ~(1<<PC6);
	if(cnt>=pwm4B) PORTC |= (1<<PC7); else PORTC &= ~(1<<PC7);
	cnt++;	
}

void TWI_init_slave(void) // Function to initilaize slave
{
    TWAR=0x20;    // Fill slave address to TWAR
} 

void TWI_match_read_slave(void) //Function to match the slave address and slave dirction bit(read)
{
    while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknoledgement have been received
    {
        // Get acknowlegement, Enable TWI, Clear TWI interrupt flag
        TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);    
        while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
    }
} 

void TWI_read_slave(void)
{
	unsigned char recv_data;
    // Clear TWI interrupt flag,Get acknowlegement, Enable TWI
    TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);    
    while (!(TWCR & (1<<TWINT)));    // Wait for TWINT flag
    while((TWSR & 0xF8)!=0x80);        // Wait for acknowledgement
    recv_data=TWDR;                    // Get value from TWDR
    PORTB=recv_data;                // send the receive value on PORTB
}




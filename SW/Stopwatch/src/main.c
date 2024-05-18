/*
 * main.c
 *
 *  Created on: 14 May 2024
 *      Author: Mega
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t sec=0,min=0,hour=0;

ISR (TIMER1_COMPA_vect){
	sec++;
	if(sec==60){
		sec=0;
		min++;
	}
	if(min==60){
		min=0;
		hour++;
	}
}

ISR (INT1_vect){
	TIMSK&=~(1<<OCIE1A);
}

ISR (INT2_vect){
	TIMSK|=(1<<OCIE1A);
}

ISR(INT0_vect){
	sec=0;
	min=0;
	hour=0;
}
void INT0_ENABLE(void){
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC01);
	MCUCR&=~(1<<ISC00);
}void INT1_ENABLE(void){
	GICR|=(1<<INT1);
	MCUCR|=(1<<ISC11);
	MCUCR|=(1<<ISC10);
}void INT2_ENABLE(void){
	GICR|=(1<<INT2);
	MCUCR&=~(1<<ISC2);
}


void enable_timer1 (void){
	TCCR1A=(1<<FOC1A);	/*NON PWM MODE*/
	TIMSK|=(1<<OCIE1A); 	/*ENABLE COMPARE INTERRUPT*/
	TCNT1 = 0; /*SET COUNTER VALUE To 0*/

	OCR1A=15625; /*COMPARE VALUE MEANS 1 SEC EVERY INTERRUPT*/
	TCCR1B=(1<<WGM12)|(1<<CS11)|(1<<CS10);
	/*WGM1 SET TO 1 TO OPERATE COMPARE MODE
	CS10,11 SET PRESCALER TO 64*/

}
int main(){
	/*Enabling the interrupt flag*/
	SREG=(1<<7);

	/* Adjusting the ports*/
	DDRC|=0x0F;
	DDRA|=0x3F;
	PORTC &=~ 0xF0;
	PORTA |= 0B0;
	DDRD &=~ (1<<PD3)&~(1<<PD2);
	DDRB &=~ (1<<PB2);
	PORTD|=(1<<PD2);
	PORTB|=(1<<PB2);

	/*Starting the interrupts and timer*/
	INT0_ENABLE();
	INT1_ENABLE();
	INT2_ENABLE();
	enable_timer1();

	while(1){
		PORTA=0B00000001;
		PORTC=hour/10;
		_delay_ms(1);
		PORTA=0B00000010;
		PORTC=hour%10;
		_delay_ms(1);
		PORTA=0B00000100;
		PORTC=min/10;
		_delay_ms(1);
		PORTA=0B00001000;
		PORTC=min%10;
		_delay_ms(1);
		PORTA=0B00010000;
		PORTC=sec/10;
		_delay_ms(1);
		PORTA=0B00100000;
		PORTC=sec%10;
		_delay_ms(1);
	}

}

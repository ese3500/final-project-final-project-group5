#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "uart.h"

char String[25];
int distance;
int period;
int temp;
int ended = 0;
int row1 = 1;
int row2 = 1;
int row3 = 1;
int col1 = 1;
int col2 = 1;
int col3 = 1;
int threshold = 50;
int counter = 0;

void Initialize(){
    cli();

//A0
    DDRB |= (1<<DDB3);

// Ultrasonic sensor stuff
//Trig
    DDRD |= (1<<DDD3);
    PORTD &= ~(1<<PORTD3);
//echo (pin for timer capture)
    DDRB &= ~(1<<DDB0);
    PORTB &= ~(1<<PORTB0);

//FOR DISTANCE SENSOR
//timer setup pre-scale by 1024 (timer 1)
    TCCR1B &= ~(1<<CS10);
    TCCR1B |= (1<<CS11);
    TCCR1B &= ~(1<<CS12);

// set timer 1 to normal
    TCCR1A &= ~(1<<WGM10);
    TCCR1A &= ~(1<<WGM11);
    TCCR1B &= ~(1<<WGM12);
    TCCR1B &= ~(1<<WGM13);
//rising edge
    TCCR1B |= (1<<ICES1);

//clear output capture
    TIFR1 |= (1<<ICF1);

//enable input capture interrupt
    TIMSK1 |= (1<<ICIE1);

//compare
    TCCR1A |= (1<<COM1A0);
    TCCR1A &= ~(1<<COM1A1);

// clear and enable timer overflow capture interrupt
    TIFR1 |= (1 << TOV1);
    TIMSK1 |= (1 << TOIE1);

// ROW output, COL input
    DDRD |= (1 << DDD7); // ROW1 pin as output
    DDRD |= (1 << DDD2); // ROW2 pin as output
    DDRB |= (1 << DDB1); // ROW3 pin as output
    DDRD &= ~(1 << DDD6); // COL1 pin as input
    DDRD &= ~(1 << DDD5); // COL2 pin as input
    DDRD &= ~(1 << DDD4); // COL3 pin as input
// Set rows and cols to high
    PORTD |= (1<<PORTD7);
    PORTD |= (1<<PORTD2);
    PORTB |= (1<<PORTB1);

// Set pin change interrupt
    PCICR |= (1<<PCIE0);
    PCMSK0 |= (1<<PCINT0);

    sei();
}

ISR(TIMER1_OVF_vect){
    counter++;
    if(counter % 3 == 0){
        PORTB |= (1 << PORTB1);
        PORTD &= ~(1 << PORTD7);
        row1 = 0;
        row3 = 1;
    } else if(counter % 3 == 1){
        PORTD |= (1 << PORTD7);
        PORTD &= ~(1 << PORTD2);
        row1 = 1;
        row2 = 0;
    } else{
        PORTD |= (1 << PORTD2);
        PORTB &= ~(1 << PORTB1);
        row3 = 0;
        row2 = 1;
    }
}

// interrupt for rising and falling edge
ISR(TIMER1_CAPT_vect)
{
    if (TCCR1B & (1 << ICES1)) {
        temp = TCNT1;
        ended = 1;
        TCCR1B &= ~(1 << ICES1);
    } else {
        period = TCNT1 - temp;
        ended = 0;
        TCCR1B |= (1<<ICES1);
    }
}

// COL1 Interrupt
ISR(PCINT0_vect){
    if(!(PIND & (1 << PIND6))){
        col1 = 0;
    } else {
        col1 = 1;
    }

    if(!(PIND & (1 << PIND5))){
        col2 = 0;
    } else {
        col2 = 1;
    }

    if(!(PIND & (1 << PIND4))){
        col3 = 0;
    } else {
        col3 = 1;
    }
}

int main(void)
{
    Initialize();
    UART_init(BAUD_PRESCALER);
    while (1)
    {
        if (ended == 0) {
            PORTD |= (1<<PORTD3);
            _delay_us(100);
            PORTD &= ~(1<<PORTD3);
            ended = 1;
        }
        long value = (period*8.62)/1000;
sprintf(String, "%d \n", value);
 UART_putstring(String);
        _delay_ms(1000);


        if(value <= 15){
            PORTB &= ~(1 << PORTB3);
        } else {
            PORTB |= (1 << PORTB3);
        }


        if(row1 == 0){
// UART_putstring("ROW 1 \n");
            if(col1 == 0){
                UART_putstring("1 \n");
                _delay_ms(1000);
            } else if(col2 == 0){
                UART_putstring("2 \n");
                _delay_ms(1000);
            } else if(col3 == 0){
                UART_putstring("3 \n");
                _delay_ms(1000);
            }
        } else if(row2 == 0){
// UART_putstring("ROW 2 \n");
            if(col1 == 0){
                UART_putstring("4 \n");
                _delay_ms(1000);
            } else if(col2 == 0){
                UART_putstring("5 \n");
                _delay_ms(1000);
            } else if(col3 == 0){
                UART_putstring("6 \n");
                _delay_ms(1000);
            }
        } else if(row3 == 0){
// UART_putstring("ROW 3 \n");
            if(col1 == 0){
                UART_putstring("7 \n");
                _delay_ms(1000);
            } else if(col2 == 0){
                UART_putstring("8 \n");
                _delay_ms(1000);
            } else if(col3 == 0){
                UART_putstring("9 \n");
                _delay_ms(1000);
            }
        }

// _delay_ms(1000);
    }
}

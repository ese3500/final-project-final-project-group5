/*
 * FinalProjectNewVersion.c
 *
 * Created: 4/14/2023 12:41:07 PM
 * Author : luoli
 */

#define F_CPU 16000000UL
#define BAUD_RATE 9600

#include <avr/io.h>
#include <stdio.h>

#include "usb.h"
#include "keycodes.h"
#include "hid_kbd.h"

void Initialize(){
    usb_init();

    DDRE &= ~(1<<DDE6); // ROW 1
    DDRD &= ~(1<<DDD7); // ROW 2
    DDRC &= ~(1<<DDC6); // ROW 3

    DDRB &= ~(1<<DDB4); // COL 1
    DDRB &= ~(1<<DDB5); // COL 2
    DDRB &= ~(1<<DDB6); // COL 3
    DDRC |= (1 << DDC7);
}

int main(void)
{
    Initialize();
    /* Replace with your application code */
    while (1)
    {
        if(!(PINB & (1 << PINB4)) && !(PINE & (1 << PINE6))){
            press_key(KEY_T, 0);
            press_key(KEY_G, 0);
            press_key(KEY_A, 0);
            PORTC |= (1 << PORTC7);
        }

        if(!(PINB & (1 << PINB4)) && !(PIND & (1 << PIND7))){
            press_key(KEY_T, 0);
            press_key(KEY_D, 0);
            press_key(KEY_R, 0);
            PORTC |= (1 << PORTC7);
        }

        if(!(PINB & (1 << PINB4)) && !(PINC & (1 << PINC6))){
            press_key(KEY_P, 0);
            press_key(KEY_V, 0);
            PORTC |= (1 << PORTC7);
        }

        if(!(PINB & (1 << PINB5)) && !(PINE & (1 << PINE6))){
            press_key(KEY_D, 0);
            press_key(KEY_I, 0);
            PORTC |= (1 << PORTC7);
        }

        if(!(PINB & (1 << PINB5)) && !(PIND & (1 << PIND7))){
            press_key(KEY_P, 0);
            press_key(KEY_F, 0);
            PORTC |= (1 << PORTC7);
        }

        if(!(PINB & (1 << PINB5)) && !(PINC & (1 << PINC6))){
            press_key(KEYPAD_ASTERIX, 0);
            PORTC |= (1 << PORTC7);
        }

        if(!(PINB & (1 << PINB6)) && !(PINE & (1 << PINE6))){
            press_key(KEY_J, 0);
            press_key(KEY_C, 0);
            PORTC |= (1 << PORTC7);
        }

        if(!(PINB & (1 << PINB6)) && !(PIND & (1 << PIND7))){
            press_key(KEY_P, 0);
            press_key(KEY_I, 0);
            PORTC |= (1 << PORTC7);
        }
        if(!(PINB & (1 << PINB6)) && !(PINC & (1 << PINC6))){
            press_key(KEY_E, 0);
            press_key(KEY_W, 0);
            PORTC |= (1 << PORTC7);
        }

// TGA
// TDR
// PV
// DI
// PW
// KEYPAD_ASTERIX
// KEY_PRINTSCREEN

        PORTC &= ~(1 << PORTC7);

    }
}
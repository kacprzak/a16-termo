/*
 * main.c
 *
 *  Created on: 28-03-2013
 *      Author: Marcin Kacprzak
 */
#include "ds18b20.h"
#include "led.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


//#define KEY PINA0

//#define DEBUG

//------------------------------------------------------------------------------

int8_t nthDigit(int x, uint8_t n) {
    while (n--) {
        x /= 10;
    }
    return (x % 10);
}

//------------------------------------------------------------------------------

#ifdef DEBUG
int8_t nthNibble(int x, uint8_t n)
{
    while (n--) {
        x >>= 4;
    }
    return (x & 0x0f);
}
#endif

//------------------------------------------------------------------------------

void init_ports() {
    DDRA = 0x00;  /* Set porta A as input */
    PORTA = 0xff; /* Pull up */

    DDRB = 0xff;  /* Set port B as output */

    DDRD = 0xff;  /* Set port D as output */
    PORTD = 0x00;
}

//------------------------------------------------------------------------------
// initialize timer, interrupt and variable
void init_timer0() {
    // Konfiguracja licznika
    // Tryb CTC, Preskaler 1024
    TCCR0 |= (1 << WGM01) | (1 << CS02) | (1 << CS00);

    // Przerwanie OC0
    TIMSK |= (1 << OCIE0);

    const int counter_frq = F_CPU / 1024u;
    // Czestotliwosc odswierzania
    OCR0 = (counter_frq / 200u) - 1; // 200 Hz (50 Hz per digit)
}

//------------------------------------------------------------------------------

void set_led_value(int16_t temper) {
    static int16_t last_temperature = 0;
    if (last_temperature == temper)
        return;

    last_temperature = temper;

#ifndef DEBUG
    int16_t fraction = (temper & 0x0f); // extract first nibble;
    fraction = ((fraction * 100) / 16);
    temper = (temper >> 4); // drop first (fraction) nibble;

    led_set_value(
            nthDigit(temper, 1),
            nthDigit(temper, 0),
            nthDigit(fraction, 1),
            nthDigit(fraction, 0)
            );
#else
    led_set_value(
            nthNibble(temper, 3),
            nthNibble(temper, 2),
            nthNibble(temper, 1),
            nthNibble(temper, 0)
            );
#endif
}

//------------------------------------------------------------------------------

// this ISR is fired whenever a match occurs
ISR (TIMER0_COMP_vect)
{
    led_update();
}

//------------------------------------------------------------------------------

int main(void) {
    /* Data from ds18b20 */
    unsigned char ds18b20_pad[9];

    init_ports();
    init_timer0();
    sei();

#ifndef DEBUG
    led_set_dot_on(2, 1);
#endif

    for (;;) {
        /* Funkcja 'ds18b20_ConvertT' wysyła do układu ds18b20
         polecenie pomiaru */
        if (ds18b20_ConvertT()) {

            /* 750ms - czas konwersji */
            _delay_ms(750);

            /* Odczyt z układu ds18b20, dane zapisywane są w tablicy ds18b20_pad.
             Dwie pierwsze pozycje w tablicy to kolejno mniej znaczący bajt i bardziej
             znaczący bajt wartość zmierzonej temperatury */
            ds18b20_Read(ds18b20_pad);

            int16_t temperature = ((ds18b20_pad[1] << 8) + (ds18b20_pad[0]));
            set_led_value(temperature);
        }
        _delay_ms(750);
    }

    return 0;
}

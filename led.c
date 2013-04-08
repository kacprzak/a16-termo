/*
 * led.c
 *
 *  Created on: 07-04-2013
 *      Author: Marcin Kacprzak
 */

#include "led.h"

static const int8_t led_numbers[] = {
    LED_0, LED_1, LED_2, LED_3, LED_4,
    LED_5, LED_6, LED_7, LED_8, LED_9,
    LED_a, LED_b, LED_c, LED_d, LED_e, LED_f
};

static int8_t led_value[4];
static int8_t led_dots_on[4];

//------------------------------------------------------------------------------

void led_set_value(int8_t digit3, int8_t digit2, int8_t digit1, int8_t digit0)
{
    led_value[0] = led_numbers[digit0];
    led_value[1] = led_numbers[digit1];
    led_value[2] = led_numbers[digit2];
    led_value[3] = led_numbers[digit3];
}

//------------------------------------------------------------------------------

void led_set_dot_on(int8_t dotnum, int8_t on)
{
    if (on)
        led_dots_on[dotnum] = LED_DOT;
    else
        led_dots_on[dotnum] = 0x00;
}

//------------------------------------------------------------------------------

void led_update()
{
    static uint8_t digit = 0;
    digit %= 4;

    PORTB = ~(led_value[digit] | led_dots_on[digit]);
    PORTD = ~(1<<digit);

    ++digit;
}

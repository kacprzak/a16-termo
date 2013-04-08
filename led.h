/*
 * led.h
 *
 *  Created on: 28-03-2013
 *      Author: Marcin Kacprzak
 */

#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

//   -- A --
//  |       |
//  F       B
//  |       |
//   -- G --
//  |       |
//  E       C
//  |       |
//   -- D --  H

// HGFE BCBA
#define LED_0   0x3f // 0011 1111
#define LED_1   0x06 // 0000 0110
#define LED_2   0x5b // 0101 1011
#define LED_3   0x4f // 0100 1111
#define LED_4   0x66 // 0110 0110
#define LED_5   0x6d // 0110 1101
#define LED_6   0x7d // 0111 1101
#define LED_7   0x07 // 0000 0111
#define LED_8   0x7f // 0111 1111
#define LED_9   0x6f // 0110 1111

#define LED_a   0x77 // 0111 0111
#define LED_b   0x7c // 0111 1100
#define LED_c   0x58 // 0101 1000
#define LED_d   0x5e // 0101 1110
#define LED_e   0x79 // 0111 1001
#define LED_f   0x71 // 0111 0001

#define LED_DOT 0x80 // 1000 0000
#define LED_DEG 0x63 // 0110 0011

void led_set_value(int8_t digit3, int8_t digit2, int8_t digit1, int8_t digit0);

void led_set_dot_on(int8_t dotnum, int8_t on);

void led_update();

#endif /* LED_H_ */

/**
 * @author  Pedro Botelho
 * @file    Switch_Led_Interrupt.c
 * @brief   If SW1, in PTD4, is pressed, light LED_GREEN_D1, PTC8.
 * 			If SW2 is pressed, PTA12, light LED_GREEN_D3, PTA13.
 *
 * 			Use of Interrupt Technique.
 */

#include <Switch_Led_Interrupt.h>

#include "MKL25Z4.h"

int main(void) {
    init_pins();
    while(1) {

    }
}

void init_pins(void) {
	// Enable Clock for PORTA, PORTC and PORTD
	SIM->SCGC5 |= (1U << 9) | (1U << 11) | (1U << 12);

	// Set GPIO Function for the pins
	SW1_PORT->PCR[SW1_PIN] |= (0b001U << 8);
	SW2_PORT->PCR[SW2_PIN] |= (0b001U << 8);
	LED_D1_PORT->PCR[LED_D1_PIN] |= (0b001U << 8);
	LED_D3_PORT->PCR[LED_D3_PIN] |= (0b001U << 8);

	// Initialize buttons as input
	SW1_GPIO->PDDR &= ~SW1_MASK;
	SW2_GPIO->PDDR &= ~SW2_MASK;

	// Initialize LEDs as output
	LED_D1_GPIO->PDDR |= LED_D1_MASK;
	LED_D3_GPIO->PDDR |= LED_D3_MASK;

	// Config Interrupt for the Buttons (Interrupt in Either Edge)
	SW1_PORT->PCR[SW1_PIN] |= (0b1011U << 16);
	SW2_PORT->PCR[SW2_PIN] |= (0b1011U << 16);

	// Enable Interrupt for PORTA and PORTD
	NVIC->ISER[0] |= (1U << 30) | (1U << 31);
}

/*
 * ISR for PORTA peripheral. Checks if PTA12 is high.
 * If so, toggle the level at PTA13.
 */
void PORTA_IRQHandler(void) {
	if(SW2_PORT->PCR[SW2_PIN] & (1U << 24)) {
		SW2_PORT->PCR[SW2_PIN] |= (1U << 24);
		LED_D3_GPIO->PTOR |= LED_D3_MASK;
	}
}

/*
 * ISR for PORTD peripheral. Checks if PTD4 is high.
 * If so, toggle the level at PTC8.
 */
void PORTD_IRQHandler(void) {
	if(SW1_PORT->PCR[SW1_PIN] & (1U << 24)) {
		SW1_PORT->PCR[SW1_PIN] |= (1U << 24);
		LED_D1_GPIO->PTOR |= LED_D1_MASK;
	}
}

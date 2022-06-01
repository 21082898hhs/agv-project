/*
 * h_bridge.c - XvR 2020
 *
 * Use 8-bit timer. Uses interrupts in order to be able
 * to use the pins on the multifunction shield
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "h_bridge.h"

ISR(TIMER0_OVF_vect)
{
	if (OCR0A == 0 && OCR0B == 0)
	{
		PORT_RPWM_RV &= ~(1<<PIN_RPWM_RV);
		PORT_LPWM_RV &= ~(1<<PIN_LPWM_RV);
		PORT_RPWM_RA &= ~(1<<PIN_RPWM_RA);
		PORT_LPWM_RA &= ~(1<<PIN_LPWM_RA);
	}
	else if (OCR0A != 0)
	{
		PORT_LPWM_RV &= ~(1<<PIN_LPWM_RV);
		PORT_RPWM_RV |= (1<<PIN_RPWM_RV);
		PORT_LPWM_RA &= ~(1<<PIN_LPWM_RA);
		PORT_RPWM_RA |= (1<<PIN_RPWM_RA);
	}
	else if (OCR0B != 0)
	{
		PORT_RPWM_RV &= ~(1<<PIN_RPWM_RV);
		PORT_LPWM_RV |= (1<<PIN_LPWM_RV);
		PORT_RPWM_RA &= ~(1<<PIN_RPWM_RA);
		PORT_LPWM_RA |= (1<<PIN_LPWM_RA);
	}
}

ISR(TIMER0_COMPA_vect)
{
	if (OCR0A != 255)
	{
		PORT_RPWM_RV &= ~(1<<PIN_RPWM_RV);
		PORT_RPWM_RA &= ~(1<<PIN_RPWM_RA);
	}
}

ISR(TIMER0_COMPB_vect)
{
	if (OCR0B != 255)
	{
	    PORT_LPWM_RV &= ~(1<<PIN_LPWM_RV);
		PORT_LPWM_RA &= ~(1<<PIN_LPWM_RA);
	}
}

ISR(TIMER2_OVF_vect)
{
	if (OCR2A == 0 && OCR2B == 0)
	{
		PORT_RPWM_LV &= ~(1<<PIN_RPWM_LV);
		PORT_LPWM_LV &= ~(1<<PIN_LPWM_LV);
		PORT_RPWM_LA &= ~(1<<PIN_RPWM_LA);
		PORT_LPWM_LA &= ~(1<<PIN_LPWM_LA);
	}
	else if (OCR2A != 0)
	{
		PORT_LPWM_LV &= ~(1<<PIN_LPWM_LV);
		PORT_RPWM_LV |= (1<<PIN_RPWM_LV);
		PORT_LPWM_LA &= ~(1<<PIN_LPWM_LA);
		PORT_RPWM_LA |= (1<<PIN_RPWM_LA);
	}
	else if (OCR2B != 0)
	{
		PORT_RPWM_LV &= ~(1<<PIN_RPWM_LV);
		PORT_LPWM_LV |= (1<<PIN_LPWM_LV);
		PORT_RPWM_LA &= ~(1<<PIN_RPWM_LA);
		PORT_LPWM_LA |= (1<<PIN_LPWM_LA);
	}
}

ISR(TIMER2_COMPA_vect)
{
	if (OCR2A != 255)
	{
		PORT_RPWM_LV &= ~(1<<PIN_RPWM_LV);
		PORT_RPWM_LA &= ~(1<<PIN_RPWM_LA);
	}
}

ISR(TIMER2_COMPB_vect)
{
	if (OCR2B != 255)
	{
	    PORT_LPWM_LV &= ~(1<<PIN_LPWM_LV);
		PORT_LPWM_LA &= ~(1<<PIN_LPWM_LA);
	}
}

void init_h_bridge(void)
{
	// Config pins as output
	DDR_RPWM_RV |= (1<<PIN_RPWM_RV);
	DDR_LPWM_RV |= (1<<PIN_LPWM_RV);
	DDR_RPWM_RA |= (1<<PIN_RPWM_RA);
	DDR_LPWM_RA |= (1<<PIN_LPWM_RA);
	DDR_RPWM_LV |= (1<<PIN_RPWM_LV);
	DDR_LPWM_LV |= (1<<PIN_LPWM_LV);
	DDR_RPWM_LA |= (1<<PIN_RPWM_LA);
	DDR_LPWM_LA |= (1<<PIN_LPWM_LA);

	// Output low
	PORT_RPWM_RV &= ~(1<<PIN_RPWM_RV);
	PORT_LPWM_RV &= ~(1<<PIN_LPWM_RV);
	PORT_RPWM_RA &= ~(1<<PIN_RPWM_RA);
	PORT_LPWM_RA &= ~(1<<PIN_LPWM_RA);

	// Use mode 0, clkdiv = 64
	TCCR0A = 0;
	TCCR0B = (0<<CS02) | (1<<CS01) | (1<<CS00);

	// Disable PWM output
	OCR0A = 0;
	OCR0B = 0;

	// Interrupts on OCA, OCB and OVF
	TIMSK0 = (1<<OCIE0B) | (1<<OCIE0A) | (1<<TOIE0);

	// Use mode 0, clkdiv = 64
	TCCR2A = 0;
	TCCR2B = (0<<CS22) | (1<<CS21) | (1<<CS20);

	// Disable PWM output
	OCR2A = 0;
	OCR2B = 0;

	// Interrupts on OCA, OCB and OVF
	TIMSK2 = (1<<OCIE2B) | (1<<OCIE2A) | (1<<TOIE2);

	sei();
}

void h_bridge_set_percentage_rechts(signed char percentage)
{
	if (percentage >= -100 && percentage <= 100)
	{
		if (percentage >= 0)
		{
			// Disable LPWM, calculate RPWM
			OCR0B = 0;
			OCR0A = (255*percentage)/100;
		}
		else // percentage < 0
		{
			// Disable RPWM, calculate LPWM
			OCR0A = 0;
			OCR0B = (255*percentage)/-100;
		}
	}
}

void h_bridge_set_percentage_links(signed char percentage)
{
	if (percentage >= -100 && percentage <= 100)
	{
		if (percentage >= 0)
		{
			// Disable LPWM, calculate RPWM
			OCR2B = 0;
			OCR2A = (255*percentage)/100;
		}
		else // percentage < 0
		{
			// Disable RPWM, calculate LPWM
			OCR2A = 0;
			OCR2B = (255*percentage)/-100;
		}
	}
}

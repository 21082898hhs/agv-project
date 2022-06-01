/*
 * h_bridge.h - XvR 2020
 */

#ifndef _H_BRIDGE_H_
#define _H_BRIDGE_H_

// These pins are available on the shield via the header:
//
//		Mega	Uno
// digital 5	PE3	PD5
// digital 6	PH3	PD6
// digital 9	PH6	PB1
// analog 5	PF5	PC5

// The settings below are for the Mega, modify
// in case you want to use other pins
#define PORT_RPWM_RV	PORTH
#define PIN_RPWM_RV 	PH3         //pin d2
#define DDR_RPWM_RV 	DDRH

#define PORT_LPWM_RV	PORTE
#define PIN_LPWM_RV 	PE5         //pin d3
#define DDR_LPWM_RV 	DDRE

#define PORT_RPWM_RA	PORTG
#define PIN_RPWM_RA 	PG5         //pin d4
#define DDR_RPWM_RA 	DDRF

#define PORT_LPWM_RA	PORTE
#define PIN_LPWM_RA 	PE3         //pin d5
#define DDR_LPWM_RA 	DDRE

#define PORT_RPWM_LV	PORTH
#define PIN_RPWM_LV 	PH6         //pin d10
#define DDR_RPWM_LV 	DDRH

#define PORT_LPWM_LV	PORTB
#define PIN_LPWM_LV 	PB5         //pin d11
#define DDR_LPWM_LV 	DDRB

#define PORT_RPWM_LA	PORTB
#define PIN_RPWM_LA 	PB6         //pin d12
#define DDR_RPWM_LA 	DDRB

#define PORT_LPWM_LA	PORTB
#define PIN_LPWM_LA 	PB7         //pin d13
#define DDR_LPWM_LA 	DDRB

void init_h_bridge(void);
void h_bridge_set_percentage_rechts(signed char percentage);
void h_bridge_set_percentage_links(signed char percentage);

#endif /* _H_BRIDGE_H_ */

/*

 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "LCD-AVR-4d.h"
#include "h_bridge.h"

#define _BV(bit) (1 << (bit))

#define IRsensor 4 //E4 --> Digitaal 2
#define detected 1
#define notDetected 0

int rising, falling;
volatile int counts;
volatile int dist;
int us_per_count;

ISR (TIMER4_CAPT_vect)
{
       if (TCCR4B & (1<<ICES4)) // On rising edge
      {
        TCCR4B &= ~(1<<ICES4); // Next time detect falling edge
        rising = ICR4; // Save current count
      }
      else // On falling edge
      {
        TCCR4B |= (1<<ICES4); // Next time detect falling edge
        falling = ICR4; // Save current count
        counts = falling - rising;
        //dist = us_per_count * counts / 58; //tijd van licht = 0,034cm/us * tijd van echo / 2 = 1/58 * tijd
        dist = us_per_count * counts * 0.17165;
      }

}

ISR (TIMER4_COMPA_vect)
{
        PORTC |= 1 << PINC4;
        _delay_us(10);
        PORTC &= ~(1 << PINC4);
}


void init_timer4()
{
  TCCR4A |= (1 << WGM41);
  TCCR4B |= (1 << CS41 | 1 << CS40); // prescaler 64
  TIMSK4 |= (1 << OCIE4A | 1 << ICIE4);
  TCCR4B |= (1 << ICES4); // Input capture on rising edge
  OCR4A = 13;
  sei();
  us_per_count = 4; // 16MHz / 64 = 25000 counts/second => 1000000/250000
}

void init(void)
{
    init_h_bridge();
}

int main(void)
{
    static int IR_value, IR_oldvalue;

    DDRE &= ~(1<<IRsensor);
    lcd_D7_ddr |= (1<<lcd_D7_bit);
    lcd_D6_ddr |= (1<<lcd_D6_bit);
    lcd_D5_ddr |= (1<<lcd_D5_bit);
    lcd_D4_ddr |= (1<<lcd_D4_bit);
    lcd_E_ddr |= (1<<lcd_E_bit);
    lcd_RS_ddr |= (1<<lcd_RS_bit);

// initialize the LCD controller as determined by the defines (LCD instructions)
    lcd_init_4d();
    lcd_write_instruction_4d(xPosition(0) | lcd_LineOne);
    lcd_write_string_4d("Distance: ");


    DDRC |= 1 << PINC4;
    PORTC &= ~(1 << PINC4);

    init_timer4();

    init();

    while(1)
    {
        IR_oldvalue = IR_value;
        if (PINE & (1<<IRsensor))
        {
            IR_value = notDetected;
        }
        else
        {
            IR_value = detected;
        }


        if(IR_value == IR_oldvalue)
        {
            if(IR_value == detected)
            {

                lcd_write_instruction_4d(xPosition(0) | lcd_LineTwo);
                lcd_write_string_4d("Object detected ");
            }

            if(IR_value == notDetected)
            {
                lcd_write_instruction_4d(xPosition(0) | lcd_LineTwo);
                lcd_write_string_4d("Nothing detected");
            }
        }


        lcd_write_instruction_4d(xPosition(10) | lcd_LineOne);
        LCDWriteInt(dist, 3);

        h_bridge_set_percentage_rechts(-40);
        h_bridge_set_percentage_links(40);
    }
    return 0;
}

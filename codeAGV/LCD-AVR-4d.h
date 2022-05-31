#include <avr/io.h>
#include <util/delay.h>

#define lcd_D7_port     PORTB                   // lcd D7 connection   D52
#define lcd_D7_bit      PORTB1
#define lcd_D7_ddr      DDRB

#define lcd_D6_port     PORTB                  // lcd D6 connection   D53
#define lcd_D6_bit      PORTB0
#define lcd_D6_ddr      DDRB

#define lcd_D5_port     PORTB                   // lcd D5 connection  D50
#define lcd_D5_bit      PORTB3
#define lcd_D5_ddr      DDRB

#define lcd_D4_port     PORTB                  // lcd D4 connection   D51
#define lcd_D4_bit      PORTB2
#define lcd_D4_ddr      DDRB

#define lcd_E_port      PORTL                  // lcd Enable pin  --> D49 (D47)
#define lcd_E_bit       PORTL2
#define lcd_E_ddr       DDRL

#define lcd_RS_port     PORTL                   // lcd RegisterSelect --> D48 (D46)
#define lcd_RS_bit      PORTL3
#define lcd_RS_ddr      DDRL

// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2
//#define   lcd_LineThree   0x14                  // start of line 3 (20x4)
//#define   lcd_lineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

// LCD instructions
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000001          // set cursor position

#define xPosition(c)  (0b10000000 + c)
// Program ID


// Function Prototypes
void lcd_write_4(uint8_t);
void lcd_write_instruction_4d(uint8_t);
void lcd_write_character_4d(uint8_t);
void lcd_write_string_4d(const char *msg);
void lcd_init_4d(void);
void LCDWriteInt(int val,unsigned int field_length);

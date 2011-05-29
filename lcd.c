/////////////////////////////////
// LCD Module Interface        //
// for your average 2 line LCD //
// By Tim K                    //
/////////////////////////////////

#include <avr/io.h>                        // include input and output functions
#include <util/delay.h>                    // include functions for accurate delays
#include <stdio.h>

#define LCD_PORT PORTA                     // customised to our hardware. should be moved to config.h
#define LCD_RS_BIT 0                       // customised to our hardware. should be moved to config.h
#define LCD_RW_BIT 6                       // customised to our hardware. should be moved to config.h
#define LCD_E_BIT 1                        // customised to our hardware. should be moved to config.h
#define LCD_DATAMASK 0b00111100            // customised to our hardware. should be moved to config.h
#define LCD_CONTROLS ~LCD_DATAMASK

#define RS0  LCD_PORT &= ~(1<<LCD_RS_BIT)             // Register select line low, select the control register
#define RS1  LCD_PORT |=  (1<<LCD_RS_BIT)             // Register select line high, select the data register
#define RW0  LCD_PORT &= ~(1<<LCD_RW_BIT)             // Write mode
#define RW1  LCD_PORT |=  (1<<LCD_RW_BIT)             // Read mode
#define E0   LCD_PORT &= ~(1<<LCD_E_BIT)              // Stop data write
#define E1   LCD_PORT |=  (1<<LCD_E_BIT)              // Start data write


void lcd_set_controls_4bit(char controlvar) {                // wait for LCD to be ready
  char NIB1;
  char NIB2;
  NIB1 = ((controlvar&0b11110000)>>4);                          // set the first nibble
  NIB2 = (controlvar&0b00001111);                               // set the second nibble
  RW0;                                                          // enable writing mode
  RS0;                                                          // enable control register
  LCD_PORT = (LCD_PORT&LCD_CONTROLS)|((NIB1<<2)&LCD_DATAMASK);  // set the first nibble bits
  E1;                                                           // write data to LCD
  _delay_us(1);                                                 // delay for signal
  E0;                                                           // stop data write
  _delay_ms(20);                                                // delay for signal
  LCD_PORT = (LCD_PORT&LCD_CONTROLS)|((NIB2<<2)&LCD_DATAMASK);  // set the second nibble bits
  E1;                                                           // write data to LCD
  _delay_us(1);                                                 // delay for signal
  E0;                                                           // stop data write
  _delay_ms(20);                                                // delay for signal
  RS1;                                                          // select the data register (disables control register)
}

void lcd_set_controls_8bit(char controlvar) {                      // wait for LCD to be ready
  RW0;                                                                // enable writing mode
  RS0;                                                                // enable control register
  LCD_PORT = (LCD_PORT&LCD_CONTROLS)|((controlvar<<2)&LCD_DATAMASK);  // set the bits
  E1;                                                                 // write data to LCD
  _delay_us(1);                                                       // delay for signal
  E0;                                                                 // stop data write
  _delay_ms(20);                                                      // delay for signal
  RS1;                                                                // select the data register (disables control register)
}

void lcd_set_home(void) {
    lcd_set_controls_4bit(0b00000001); // Go to home position. Return the DDRAM input to home
}

void lcd_clear_display(void) {
    lcd_set_controls_8bit(0b00000011); // Init
    lcd_set_controls_8bit(0b00000011); // Init
    lcd_set_controls_8bit(0b00000011); // Init
    lcd_set_controls_8bit(0b00000010); // Enable 4bit mode
    lcd_set_controls_4bit(0b00101000); // Enable 4bit mode, all 4 lines, default character.
    lcd_set_controls_4bit(0b00001100); // Enable the display. Disable the cursor. Do not blink cursor.
    lcd_set_controls_4bit(0b00000001); // Go to home position.
    lcd_set_controls_4bit(0b00000110); // Increment and do not shift display.
}

void lcd_set_line(char x) {
  switch (x) {
    case 0:
            lcd_set_controls_4bit((1<<7)|0x00);    // sets the position of the DDRAM input
            break;
    case 1:
            lcd_set_controls_4bit((1<<7)|0x40);    // sets the position of the DDRAM input
            break;
    case 2:
            lcd_set_controls_4bit((1<<7)|0x14);    // sets the position of the DDRAM input
            break;
    case 3:
            lcd_set_controls_4bit((1<<7)|0x54);    // sets the position of the DDRAM input
            break;
  }
}

void lcd_write(char writeval) {
    char NIB1;
    char NIB2;
    NIB1 = (writeval&0b11110000) >> 4;
    NIB2 = (writeval&0b00001111);
    LCD_PORT = (LCD_PORT&LCD_CONTROLS)|((NIB1<<2)&LCD_DATAMASK);  // set the bits
    E1;                                                       // write data to LCD
    _delay_us(1);                                             // delay for signal
    E0;                                                       // stop writing data
    _delay_ms(3);
    LCD_PORT = (LCD_PORT&LCD_CONTROLS)|((NIB2<<2)&LCD_DATAMASK);  // set the bits
    E1;                                                       // write data to LCD
    _delay_us(1);                                             // delay for signal
    E0;                                                       // stop writing data
    _delay_ms(3);
}

void lcd_write_line(char *s) {
  char t;
  for(t = 0; t<20; t++) {       // reads 14 characters from the passed string
  if(*s) {                      // if there is a character
  lcd_write(*s);                // write it to the LCD
  s++;                          // and go to the next character
  }
  }
}

void lcd_cnc_init(void) {
    DDRA  = 0xFF;
    lcd_clear_display();                    // clear and initialise the LCD
    lcd_set_home();                         // sets the lcd home
    lcd_set_line(0);                        // sets the screen position, line 1
    lcd_write_line("HackMelbourne CNC");    // writes this string to the LCD
    lcd_set_line(1);                        // sets the screen position, line 2
    lcd_write_line("GRBL loaded:");         // writes this string to the LCD
    lcd_set_line(2);                        // sets the screen position, line 3
    lcd_write_line("XPos, Ypos, Zpos");     // writes this string to the LCD
    lcd_set_line(3);                        // sets the screen position, line 4
    lcd_write_line("XNext, YNext, Znext");  // writes this string to the LCD
}
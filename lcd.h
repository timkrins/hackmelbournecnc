/*
LCD Module Interface
HackMelbourne CNC 4line display
Questions? Ask Tim K
*/

#ifndef lcd_h
#define lcd_h

#include "config.h"

#define LCD_DATAMASK (1<<LCD_DATA_BIT_1)|(1<<LCD_DATA_BIT_2)|(1<<LCD_DATA_BIT_3)|(1<<LCD_DATA_BIT_4)
#define LCD_CONTROLS ~LCD_DATAMASK

#define RS0  LCD_PORT &= ~(1<<LCD_RS_BIT)             // Register select line low, select the control register
#define RS1  LCD_PORT |=  (1<<LCD_RS_BIT)             // Register select line high, select the data register
#define RW0  LCD_PORT &= ~(1<<LCD_RW_BIT)             // Write mode
#define RW1  LCD_PORT |=  (1<<LCD_RW_BIT)             // Read mode
#define E0   LCD_PORT &= ~(1<<LCD_E_BIT)              // Stop data write
#define E1   LCD_PORT |=  (1<<LCD_E_BIT)              // Start data write

#define LCD_NIBBLE ((LCD_PORT&LCD_CONTROLS)|(((((NIBBLE&0b00000001)<<LCD_DATA_BIT_1)|((NIBBLE&0b00000010)<<LCD_DATA_BIT_2))|(((NIBBLE&0b00000100)<<LCD_DATA_BIT_3)|((NIBBLE&0b00001000)<<LCD_DATA_BIT_4)))))

#define LCD_CONTROLS_NIB2 ((LCD_PORT&LCD_CONTROLS)|(((NIB2&0b00000001)<<LCD_DATA_BIT_1)|((NIB2&0b00000010)<<LCD_DATA_BIT_2)|((NIB2&0b00000100)<<LCD_DATA_BIT_3)|((NIB2&0b00001000)<<LCD_DATA_BIT_4)))

#endif

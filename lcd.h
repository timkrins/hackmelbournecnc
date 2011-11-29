#ifndef lcd_h
#define lcd_h

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

char controlvar;
char x;
char writeval;

void lcd_set_controls_4bit();
void lcd_set_line();
void lcd_write();
void lcd_write_line();
void lcd_cnc_init();
void lcd_clear();

#endif
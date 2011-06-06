/*
  config.h - compile time configuration
  Part of Grbl

  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
Specifics for the CCHS CNC machine:

The X and Y axes:
Steppers have a 1.8degree step: 200 steps per revolution.
The lead of the screw: 0.125inches = 3.175mm.
3.175 is one turn.
So, turns_per_mm = 1/3.175 = 0.3149606299212598.
steps_per_mm = (steps_per_revolution*microsteps)/turns_per_mm
             = (200*1)/0.3149606299212598.
             = 635, very close to exact.
             
The Z axis: 
*/

#ifndef config_h
#define config_h


#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#define BAUD_RATE 9600

// Updated default pin-assignments from 0.6 onwards 
// (see bottom of file for a copy of the old config)

#define STEPPERS_ENABLE_DDR     DDRC
#define STEPPERS_ENABLE_PORT    PORTC
#define STEPPERS_ENABLE_BIT     3 //PC3

#define STEPPERS_RESET_DDR     	DDRC
#define STEPPERS_RESET_PORT    	PORTC
#define STEPPERS_RESET_BIT      4 //PC4

#define STEPPING_DDR       	DDRC
#define STEPPING_PORT      	PORTC
#define X_STEP_BIT           	5 //PC5
#define Y_STEP_BIT           	6 //PC6
#define Z_STEP_BIT           	7 //PC7
#define X_DIRECTION_BIT      	2 //PC2
#define Y_DIRECTION_BIT      	1 //PC1
#define Z_DIRECTION_BIT      	0 //PC0

#define LIMIT_DDR      		DDRB
#define LIMIT_PORT     		PORTB
#define XPOS_LIMIT_BIT          0
#define XNEG_LIMIT_BIT          1
#define YPOS_LIMIT_BIT          2
#define YNEG_LIMIT_BIT          3
#define ZPOS_LIMIT_BIT          4
#define ZNEG_LIMIT_BIT          5

#define SPINDLE_ENABLE_DDR 	DDRD
#define SPINDLE_ENABLE_PORT 	PORTD
#define SPINDLE_ENABLE_BIT 	2
#define SPINDLE_DIRECTION_PORT DDRD
#define SPINDLE_DIRECTION_BIT 1

#define LCD_DDR			DDRA
#define LCD_PORT 		PORTA
#define LCD_RS_BIT 		0
#define LCD_RW_BIT 		6
#define LCD_E_BIT 		1
#define LCD_DATA_BIT_1		2
#define LCD_DATA_BIT_2		3
#define LCD_DATA_BIT_3		4
#define LCD_DATA_BIT_4		5

// The temporal resolution of the acceleration management subsystem. 
// Higher numbers give smoother acceleration but may impact performance.
#define ACCELERATION_TICKS_PER_SECOND 40L

#endif

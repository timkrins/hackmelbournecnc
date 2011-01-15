#ifndef	_CONFIG_H
#define	_CONFIG_H

/*
	CONTENTS
	
	1. Mechanical/Hardware
	2. Acceleration settings
	3. Pinouts
	4. Temperature sensors
	5. Heaters
	6. Communication options
	7. Miscellaneous
*/

/***************************************************************************\
* 1. MECHANICAL/HARDWARE                                                    *                                                                          *
\***************************************************************************/

/*
	Microcontroller type is set to atmega644p in the Makefile.
*/

/*
	CPU clock rate
*/
#ifndef	F_CPU
	#define	F_CPU	20000000L
#endif

#define	HOST

/*
	Values reflecting the gearing of your machine.
		All numbers are fixed point integers, so no more than 3 digits to the right of the decimal point, please :-)
*/

// calculate these values appropriate for your machine
// for threaded rods, this is (steps motor per turn) / (pitch of the thread)
// for belts, this is (steps per motor turn) / (number of gear teeth) / (belt module)
// half-stepping doubles the number, quarter stepping requires * 4, etc.
#define	STEPS_PER_MM_X				320.000
#define	STEPS_PER_MM_Y				320.000
#define	STEPS_PER_MM_Z				320.000

/*
	Values depending on the capabilities of your stepper motors and other mechanics.
		All numbers are integers, no decimals allowed.

		Units are mm/min
*/

// used for G0 rapid moves and as a cap for all other feedrates
#define	MAXIMUM_FEEDRATE_X		200
#define	MAXIMUM_FEEDRATE_Y		200
#define	MAXIMUM_FEEDRATE_Z		100

// used when searching endstops and as default feedrate
#define	SEARCH_FEEDRATE_X			50
#define	SEARCH_FEEDRATE_Y			50
#define	SEARCH_FEEDRATE_Z			50

/***************************************************************************\
* 2. ACCELERATION                                                           *
* IMPORTANT: choose only one! These algorithms choose when to step, trying  *
*            to use more than one will have undefined and probably          *
*            disastrous results!                                            *
\***************************************************************************/

/*
	acceleration, reprap style.
		Each movement starts at the speed of the previous command and accelerates or decelerates linearly to reach target speed at the end of the movement.
*/
// #define ACCELERATION_REPRAP


/*
	acceleration and deceleration ramping.
		Each movement starts at (almost) no speed, linearly accelerates to target speed and decelerates just in time to smoothly stop at the target. alternative to ACCELERATION_REPRAP
*/
#define ACCELERATION_RAMPING

// how fast to accelerate when using ACCELERATION_RAMPING
// smaller values give quicker acceleration
// valid range = 1 to 8,000,000; 500,000 is a good starting point
#define ACCELERATION_STEEPNESS	500000


/*
	temporal step algorithm
		This algorithm causes the timer to fire when any axis needs to step, instead of synchronising to the axis with the most steps ala bresenham.

		This algorithm is not a type of acceleration, and I haven't worked out how to integrate acceleration with it.
		However it does control step timing, so acceleration algorithms seemed appropriate

		The Bresenham algorithm is great for drawing lines, but not so good for steppers - In the case where X steps 3 times to Y's two, Y experiences massive jitter as it steps in sync with X every 2 out of 3 X steps. This is a worst-case, but the problem exists for most non-45/90 degree moves. At higher speeds, the jitter /will/ cause position loss and unnecessary vibration.
		This algorithm instead calculates when a step occurs on any axis, and sets the timer to that value.

		// TODO: figure out how to add acceleration to this algorithm
*/
// #define ACCELERATION_TEMPORAL



/***************************************************************************\
* 3. PINOUTS                                                                *
\***************************************************************************/

/*
	Machine Pin Definitions
	- make sure to avoid duplicate usage of a pin
	- comment out pins not in use, as this drops the corresponding code and makes operations faster
*/

#include	"arduino.h"

#ifndef	GEN3
	/*
		user defined pins
		adjust to suit your electronics
	*/

  #define	X_STEP_PIN						DIO21  //XSTEP is PC5... ie. DIO21
  #define	X_DIR_PIN							DIO18  //XDIR pin is PC2. DIO18
  #define	X_MIN_PIN							DIO0  // XMIN is not correct, we have an XHOME on PB0

  #define	Y_STEP_PIN						DIO22 // PC6 is DIO22
  #define	Y_DIR_PIN							DIO17 // DIO17 is PC1
  #define	Y_MIN_PIN							DIO1 // PB1

  #define	Z_STEP_PIN						DIO23 // PC7 is DIO23
  #define	Z_DIR_PIN							DIO16 // DIO16 is PC0
  #define	Z_MIN_PIN							DIO7 // PB2
	
  #define	STEPPER_ENABLE_PIN		DIO19 // PC3 (active low)
  #define STEPPER_RESET_PIN     DIO20 // PC4 (needs to be high to move) 
  
  
#else
	/*
		this is the official gen3 reprap motherboard pinout
	*/
	#define TX_ENABLE_PIN					DIO12
	#define	RX_ENABLE_PIN					DIO13

	#define	X_STEP_PIN						DIO15
	#define	X_DIR_PIN							DIO18
	#define	X_MIN_PIN							DIO20
	#define	X_MAX_PIN							DIO21
	#define	X_ENABLE_PIN					DIO19

	#define	Y_STEP_PIN						DIO23
	#define	Y_DIR_PIN							DIO22
	#define	Y_MIN_PIN							AIO6
	#define	Y_MAX_PIN							AIO5
	#define	Y_ENABLE_PIN					DIO7

	#define	Z_STEP_PIN						AIO4
	#define	Z_DIR_PIN							AIO3
	#define	Z_MIN_PIN							AIO1
	#define	Z_MAX_PIN							AIO0
	#define	Z_ENABLE_PIN					AIO2

	#define	E_STEP_PIN						DIO16
	#define	E_DIR_PIN							DIO17

	#define	SD_CARD_DETECT				DIO2
	#define	SD_WRITE_PROTECT			DIO3
#endif

/***************************************************************************\
* 5. TEMPERATURE SENSORS                                                    *
\***************************************************************************/

// how many temperature sensors do you have?
#define	NUM_TEMP_SENSORS	0

/***************************************************************************\
* 5. HEATERS                                                                *
\***************************************************************************/

// number of heaters- for GEN3, set to zero as extruder manages the heater by itself
#define	NUM_HEATERS				0

/***************************************************************************\
* 6. COMMUNICATION OPTIONS                                                  *
\***************************************************************************/

/*
	RepRap Host changes it's communications protocol from time to time and intentionally avoids backwards compatibility. Set this to the date the source code of your Host was fetched from RepRap's repository, which is likely also the build date.
	See the discussion on the reprap-dev mailing list from 11 Oct. 2010.

	Undefine it for best human readability, set it to an old date for compatibility with hosts before August 2010
*/
// #define REPRAP_HOST_COMPATIBILITY 19750101
#define REPRAP_HOST_COMPATIBILITY 20100806
// #define REPRAP_HOST_COMPATIBILITY <date of next RepRap Host compatibility break>

/*
	Xon/Xoff flow control.
		Redundant when using RepRap Host for sending GCode, but mandatory when sending GCode files with a plain terminal emulator, like GtkTerm (Linux), CoolTerm (Mac) or HyperTerminal (Windows).
		Can also be set in Makefile
*/
#define	XONXOFF



/***************************************************************************\
* 7. MISCELLANEOUS OPTIONS                                                  *
\***************************************************************************/

/*
	DEBUG
		enables /heaps/ of extra output, and some extra M-codes.
		WARNING: this WILL break most host-side talkers that expect particular responses from firmware such as reprap host and replicatorG
		use with serial terminal or other suitable talker only.
*/
// #define	DEBUG

/*
	move buffer size, in number of moves
		note that each move takes a fair chunk of ram (69 bytes as of this writing) so don't make the buffer too big - a bigger serial readbuffer may help more than increasing this unless your gcodes are more than 70 characters long on average.
		however, a larger movebuffer will probably help with lots of short consecutive moves, as each move takes a bunch of math (hence time) to set up so a longer buffer allows more of the math to be done during preceding longer moves
*/
#define	MOVEBUFFER_SIZE	8


/*
	FiveD on Arduino implements a watchdog, which has to be reset every 250ms or it will reboot the controller. As rebooting (and letting the GCode sending application trying to continue the build with a then different Home point) is probably even worse than just hanging, and there is no better restore code in place, this is disabled for now.
*/
// #define USE_WATCHDOG

/*
	analog subsystem stuff
	REFERENCE - which analog reference to use. see analog.h for choices
*/
#define	REFERENCE			REFERENCE_AVCC

/*
	this option makes the step interrupt interruptible (nested).
	this should help immensely with dropped serial characters, but may also make debugging infuriating due to the complexities arising from nested interrupts
*/
#define		STEP_INTERRUPT_INTERRUPTIBLE	1

/*
	how often we overflow and update our clock; with F_CPU=16MHz, max is < 4.096ms (TICK_TIME = 65535)
*/
#define		TICK_TIME			2 MS
#define		TICK_TIME_MS	(TICK_TIME / (F_CPU / 1000))


// this is the scaling of internally stored PID values. 1024L is a good value
#define	PID_SCALE						1024L

#endif	/* _CONFIG_H */

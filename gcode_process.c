#include	"gcode_process.h"

#include	<string.h>

#include	"gcode_parse.h"

#include	"dda_queue.h"
#include	"delay.h"
#include	"serial.h"
#include	"sermsg.h"
#include	"timer.h"
#include	"sersendf.h"
#include	"pinio.h"
#include	"debug.h"
#include	"clock.h"

// the current tool
uint8_t tool;
// the tool to be changed when we get an M6
uint8_t next_tool;


/*
public functions
*/

void zero_x(void) {
	TARGET t = startpoint;
	t.X = 0;
	t.F = SEARCH_FEEDRATE_X;
	enqueue(&t);
}

void zero_y(void) {
	TARGET t = startpoint;
	t.Y = 0;
	t.F = SEARCH_FEEDRATE_X;
	enqueue(&t);
}

void zero_z(void) {
	TARGET t = startpoint;
	t.Z = 0;
	t.F = SEARCH_FEEDRATE_Z;
	enqueue(&t);
}

/****************************************************************************
*                                                                           *
* Command Received - process it                                             *
*                                                                           *
****************************************************************************/

void process_gcode_command() {
	uint32_t	backup_f;
	
	// convert relative to absolute
	if (next_target.option_relative) {
		next_target.target.X += startpoint.X;
		next_target.target.Y += startpoint.Y;
		next_target.target.Z += startpoint.Z;
	}

	if (next_target.seen_T) {
		next_tool = next_target.T;
	}

	if (next_target.seen_G) {
		uint8_t axisSelected = 0;
		switch (next_target.G) {
			// 	G0 - rapid, unsynchronised motion
			// since it would be a major hassle to force the dda to not synchronise, just provide a fast feedrate and hope it's close enough to what host expects
			case 0:
				backup_f = next_target.target.F;
				next_target.target.F = MAXIMUM_FEEDRATE_X * 2;
				enqueue(&next_target.target);
				next_target.target.F = backup_f;
				break;
				
				//	G1 - synchronised motion
			case 1:
				enqueue(&next_target.target);
				break;
				
				//	G2 - Arc Clockwise
				// unimplemented
				
				//	G3 - Arc Counter-clockwise
				// unimplemented
				
				//	G4 - Dwell
			case 4:
				// wait for all moves to complete
				queue_wait();
				// delay
				for (;next_target.P > 0;next_target.P--) {
					ifclock(CLOCK_FLAG_10MS) {
						clock_10ms();
					}
					delay_ms(1);
				}
				break;
				
				//	G20 - inches as units
			case 20:
				next_target.option_inches = 1;
				break;
				
				//	G21 - mm as units
			case 21:
				next_target.option_inches = 0;
				break;
				
				//	G30 - go home via point
			case 30:
				enqueue(&next_target.target);
				// no break here, G30 is move and then go home
				
				//	G28 - go home
			case 28:
				queue_wait();
				
				if (next_target.seen_X) {
					zero_x();
					axisSelected = 1;
				}
				if (next_target.seen_Y) {
					zero_y();
					axisSelected = 1;
				}
				if (next_target.seen_Z) {
					zero_z();
					axisSelected = 1;
				}
				
				break;
				
				//	G90 - absolute positioning
				case 90:
					next_target.option_relative = 0;
					break;
					
					//	G91 - relative positioning
				case 91:
					next_target.option_relative = 1;
					break;
					
					//	G92 - set home
				case 92:
					// wait for queue to empty
					queue_wait();

					if (next_target.seen_X) {
						startpoint.X = current_position.X = next_target.target.X;
						axisSelected = 1;
					}
					if (next_target.seen_Y) {
						startpoint.Y = current_position.Y = next_target.target.Y;
						axisSelected = 1;
					}
					if (next_target.seen_Z) {
						startpoint.Z = current_position.Z = next_target.target.Z;
						axisSelected = 1;
					}
					if (axisSelected == 0) {
						startpoint.X = current_position.X =
						startpoint.Y = current_position.Y =
						startpoint.Z = current_position.Z = 0;
					}
					break;
					
					// unknown gcode: spit an error
				default:
					sersendf_P(PSTR("E: Bad G-code %d"), next_target.G);
					// newline is sent from gcode_parse after we return
		}
	}
	else if (next_target.seen_M) {
		switch (next_target.M) {
			// M2- program end
			case 2:
				timer_stop();
				queue_flush();
				x_disable();
				y_disable();
				z_disable();
				power_off();
				for (;;)
					wd_reset();
				break;

			// M6- tool change
			case 6:
				tool = next_tool;
				break;
				
				// M110- set line number
			case 110:
				next_target.N_expected = next_target.S - 1;
				break;
				// M111- set debug level
				#ifdef	DEBUG
			case 111:
				debug_flags = next_target.S;
				break;
				#endif
				// M112- immediate stop
			case 112:
				timer_stop();
				queue_flush();
				power_off();
				break;
				// M113- extruder PWM
				// M114- report XYZEF to host
			case 114:
				sersendf_P(PSTR("X:%ld,Y:%ld,Z:%ld,F:%ld"), current_position.X, current_position.Y, current_position.Z, current_position.F);
				// newline is sent from gcode_parse after we return
				break;
				// M115- capabilities string
			case 115:
				sersendf_P(PSTR("FIRMWARE_NAME:FiveD_on_Arduino FIRMWARE_URL:http%%3A//github.com/triffid/FiveD_on_Arduino/ PROTOCOL_VERSION:1.0 MACHINE_TYPE:Mendel EXTRUDER_COUNT:%d TEMP_SENSOR_COUNT:%d HEATER_COUNT:%d"), 1, 0, 0);
				// newline is sent from gcode_parse after we return
				break;
				
				// M190- power on
			case 190:
				power_on();
				x_enable();
				y_enable();
				z_enable();
				steptimeout = 0;
				break;
				// M191- power off
			case 191:
				x_disable();
				y_disable();
				z_disable();
				power_off();
				break;
				
			#ifdef	DEBUG
				// M140- echo off
			case 140:
				debug_flags &= ~DEBUG_ECHO;
				serial_writestr_P(PSTR("Echo off"));
				// newline is sent from gcode_parse after we return
				break;
				// M141- echo on
			case 141:
				debug_flags |= DEBUG_ECHO;
				serial_writestr_P(PSTR("Echo on"));
				// newline is sent from gcode_parse after we return
				break;
				
				// DEBUG: return current position, end position, queue
			case 250:
				sersendf_P(PSTR("{X:%ld,Y:%ld,Z:%ld,F:%lu,c:%lu}\t{X:%ld,Y:%ld,Z:%ld,F:%lu,c:%lu}\t"), current_position.X, current_position.Y, current_position.Z, current_position.F, movebuffer[mb_tail].c, movebuffer[mb_tail].endpoint.X, movebuffer[mb_tail].endpoint.Y, movebuffer[mb_tail].endpoint.Z, movebuffer[mb_tail].endpoint.F,
					#ifdef ACCELERATION_REPRAP
						movebuffer[mb_tail].end_c
					#else
						movebuffer[mb_tail].c
					#endif
					);

				print_queue();
				break;
				
				// DEBUG: read arbitrary memory location
			case 253:
				if (next_target.seen_P == 0)
					next_target.P = 1;
				for (; next_target.P; next_target.P--) {
					serwrite_hex8(*(volatile uint8_t *)(next_target.S));
					next_target.S++;
				}
				// newline is sent from gcode_parse after we return
				break;
				
				// DEBUG: write arbitrary memory locatiom
			case 254:
				sersendf_P(PSTR("%x:%x->%x"), next_target.S, *(volatile uint8_t *)(next_target.S), next_target.P);
				(*(volatile uint8_t *)(next_target.S)) = next_target.P;
				// newline is sent from gcode_parse after we return
				break;
			#endif /* DEBUG */
				// unknown mcode: spit an error
			default:
				sersendf_P(PSTR("E: Bad M-code %d"), next_target.M);
				// newline is sent from gcode_parse after we return
		} // switch (next_target.M)
	} // else if (next_target.seen_M)
} // process_gcode_command()

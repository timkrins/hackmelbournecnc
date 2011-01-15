#include	"clock.h"

#include	"pinio.h"
#include	"sersendf.h"
#include	"dda_queue.h"
#include	"timer.h"
#include	"debug.h"

void clock_250ms() {
	if (steptimeout > (30 * 4)) {
		power_off();
	}
	else
		steptimeout++;
	
	ifclock(CLOCK_FLAG_1S) {
		if (debug_flags & DEBUG_POSITION) {
			// current position
			sersendf_P(PSTR("Pos: %ld,%ld,%ld,%lu\n"), current_position.X, current_position.Y, current_position.Z, current_position.F);
			
			// target position
			sersendf_P(PSTR("Dst: %ld,%ld,%ld,%lu\n"), movebuffer[mb_tail].endpoint.X, movebuffer[mb_tail].endpoint.Y, movebuffer[mb_tail].endpoint.Z, movebuffer[mb_tail].endpoint.F);
			
			// Queue
			print_queue();
		}
	}
}

void clock_10ms() {
	// reset watchdog
	wd_reset();
	
	ifclock(CLOCK_FLAG_250MS) {
		clock_250ms();
	}
}


#include "ideal_handler.h"
#include "src/signals.h"

#include <signal.h>
#include <unistd.h>

int setup_ideal() {
	int ret = 0;
	struct sigaction act;

	sigemptyset(&act.sa_mask);	// clear mask set
	act.sa_flags = 0;		// no additional options needed

	/* PRIO_1 */
	act.sa_handler = handle_signal_1;	// set handler
	ret |= sigaction(PRIO_1, &act, NULL);	// set trap

	/* PRIO_2 */
	act.sa_handler = handle_signal_2;	// set handler
	sigaddset(&act.sa_mask, PRIO_1);	// mask lower prio
	ret |= sigaction(PRIO_2, &act, NULL);	// set trap

	/* PRIO_3 */
	act.sa_handler = handle_signal_3;	// set handler
	sigaddset(&act.sa_mask, PRIO_2);	// mask lower prio
	ret |= sigaction(PRIO_3, &act, NULL);	// set trap
	
	/* PRIO_4 */
	act.sa_handler = handle_signal_4;	// set handler
	sigaddset(&act.sa_mask, PRIO_3);	// mask lower prio
	ret |= sigaction(PRIO_4, &act, NULL);	// set trap
	
	return ret;
}

void loop_ideal() {
	while (1)
		sleep(1);
}

void run_ideal() {
	setup_ideal();
	loop_ideal();
}

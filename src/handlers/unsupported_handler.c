#include "unsupported_handler.h"

#include "src/signals.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static int interrupts[SIGNAL_COUNT] = {0, 0, 0, 0};
static void (*handlers[SIGNAL_COUNT])() = {handle_signal_1, handle_signal_2, handle_signal_3, handle_signal_4};

static void process_next() {
	for (int i = SIGNAL_COUNT - 1; i >= 0; i--) {
		if (interrupts[i]) {
			printf("Accepting interrupt %d\n", i + 1); \
			sleep(1); \
			handlers[i]();
			interrupts[i] = 0;
			break;
		}
	}	
}

#define REGISTER(prio) \
	static void register_signal_ ## prio() { \
		interrupts[prio - 1] = 1; \
}

REGISTER(1)
REGISTER(2)
REGISTER(3)
REGISTER(4)

int setup_unsupported() {
	int ret = 0;
	struct sigaction act;

	sigemptyset(&act.sa_mask);	// clear mask set
	act.sa_flags = 0;		// no additional options needed

	/* Mask all interrupts */
	sigaddset(&act.sa_mask, PRIO_1);
	sigaddset(&act.sa_mask, PRIO_2);
	sigaddset(&act.sa_mask, PRIO_3);
	sigaddset(&act.sa_mask, PRIO_4);

	act.sa_handler = register_signal_1;
	ret |= sigaction(PRIO_1, &act, NULL);

	act.sa_handler = register_signal_2;
	ret |= sigaction(PRIO_2, &act, NULL);

	act.sa_handler = register_signal_3;
	ret |= sigaction(PRIO_3, &act, NULL);

	act.sa_handler = register_signal_4;
	ret |= sigaction(PRIO_4, &act, NULL);

	return ret;
}

void loop_unsupported() {
	while (1) {
		sleep(1);
		process_next();
	}
}

void run_unsupported() {
	setup_unsupported();
	loop_unsupported();
}

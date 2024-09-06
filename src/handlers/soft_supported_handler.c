#include "soft_supported_handler.h"
#include "src/signals.h"

#include <signal.h>
#include <stdio.h>
#include <sys/_types/_null.h>
#include <sys/signal.h>
#include <unistd.h>

#define REGISTER(prio) \
	static void register_signal_ ## prio() { \
		if (current_prio < prio) { \
			handle_interrupt(prio); \
		} else { \
			register_interrupt(prio); \
		} \
	}

static int interrupts[SIGNAL_COUNT] = {0, 0, 0, 0};
static void (*handlers[SIGNAL_COUNT])() = {handle_signal_1, handle_signal_2, handle_signal_3, handle_signal_4};

int current_prio = 0;

void handle_interrupt(int prio) {
	int old_prio = current_prio;
	current_prio = prio;
	printf("Accepting signal %d\n", prio);
	sleep(1);
	handlers[prio - 1]();
	current_prio = old_prio;
}

void register_interrupt(int prio) {
	interrupts[prio - 1] = 1;
}

REGISTER(1)
REGISTER(2)
REGISTER(3)
REGISTER(4)

static void process_next() {
	for (int i = SIGNAL_COUNT - 1; i >= 0; i--) {
		if (interrupts[i]) {
			handle_interrupt(i + 1);
			interrupts[i] = 0;
			break;
		}
	}	
}

int setup_soft_supported() {
	int ret = 0;
	struct sigaction act;

	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

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

void loop_soft_supported() {
	while (1) {
		sleep(1);
		process_next();
	}
}

void run_soft_supported() {
	setup_soft_supported();
	loop_soft_supported();
}

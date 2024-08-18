#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define SLEEP_TIME 5

#define SIGNAL_COUNT 4

#define PRIO_1 SIGWINCH
#define PRIO_2 SIGINFO
#define PRIO_3 SIGUSR1
#define PRIO_4 SIGUSR2

static const int priority[SIGNAL_COUNT] = { PRIO_1, PRIO_2, PRIO_3, PRIO_4 };

/* Gets priority of signal sig.
 * If sig isn't in priority list, returns -1. */
int get_priority(int sig) {
	int res = -1;

	for (int i = 0; i < SIGNAL_COUNT; i++) {
		if (priority[i] == sig) {
			res = i;
			break;
		}
	}

	return res;
}

#define HANDLER(prio) \
	void handle_signal_ ## prio() { \
			for (int i = 0; i < SLEEP_TIME; i++) { \
			printf("Handling signal %d\n", prio); \
			sleep(1);\
		} \
		printf("Done with signal %d\n", prio);\
	} 

HANDLER(1)
HANDLER(2)
HANDLER(3)
HANDLER(4)

#endif // !SIGNALS_H

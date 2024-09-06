#include "signals.h"

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

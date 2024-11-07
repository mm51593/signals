#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define SLEEP_TIME 5

#define SIGNAL_COUNT 4

#define STARTING_PRIO	0
#define PRIO_1 SIGWINCH
#define PRIO_2 SIGINFO
#define PRIO_3 SIGUSR1
#define PRIO_4 SIGUSR2

static const int priority[SIGNAL_COUNT] = { PRIO_1, PRIO_2, PRIO_3, PRIO_4 };

/* Gets priority of signal sig.
 * If sig isn't in priority list, returns -1. */
int get_priority(int sig);

void handle_signal_1();
void handle_signal_2();
void handle_signal_3();
void handle_signal_4();

#endif // !SIGNALS_H

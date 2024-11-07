#include "hard_supported_handler.h"
#include "src/signals.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <unistd.h>

#define INTERRUPT_SIGNAL	SIGIO
#define EOI_SIGNAL		SIGCONT
#define STACK_SIZE		20

#define REGISTER(prio) \
static void register_signal_ ## prio() { \
	ic.interrupts[prio - 1] = 1; \
	check_interrupts(&ic); \
} 

struct Stack {
	int stack[STACK_SIZE];
	int stack_pointer;
};

struct Processor {
	struct Stack stack;
	int current_prio;
};

struct InterruptCircuit {
	int interrupts[SIGNAL_COUNT]; 
	void (*handlers[SIGNAL_COUNT])();
	int  current_prio;
	void (*handler_address)();
};

struct Processor proc = {
	{
		{},
		0
	},
	STARTING_PRIO
};

struct InterruptCircuit ic = {
	{0, 0, 0, 0},
	{handle_signal_1, handle_signal_2, handle_signal_3, handle_signal_4},
	0,
	NULL
};

int get_highest_prio(struct InterruptCircuit *ic) {
	for (int i = SIGNAL_COUNT - 1; i >= 0; i--) {
		if (ic->interrupts[i]) {
			return i + 1;
		}
	}
	return 0;
}

void check_interrupts(struct InterruptCircuit *ic) {
	int highest_prio = get_highest_prio(ic);
	if (highest_prio > ic->current_prio) {
		ic->current_prio = highest_prio;
		ic->handler_address = ic->handlers[highest_prio - 1];
		kill(getpid(), INTERRUPT_SIGNAL);
	}
}

void lower_interrupt_flag(struct InterruptCircuit *ic, int prio) {
	ic->interrupts[prio - 1] = 0;
}

void finalize_interrupt() {
	// lower_interrupt_flag(&ic, ic.current_prio);
	ic.current_prio = proc.current_prio;
	check_interrupts(&ic);
}

int stack_get(struct Stack *stack) {
	return stack->stack[stack->stack_pointer - 1];
}

void stack_push(struct Stack *stack, int element) {
	stack->stack[stack->stack_pointer++] = element;
}

int stack_pop(struct Stack *stack) {
	return stack->stack[--stack->stack_pointer];
}

void store_context(struct Stack *stack, int prio) {
	stack_push(stack, prio);	
}

int restore_context(struct Stack *stack) {
	return stack_pop(stack);
}

sigset_t get_full_mask() {
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, PRIO_1);
	sigaddset(&mask, PRIO_2);
	sigaddset(&mask, PRIO_3);
	sigaddset(&mask, PRIO_4);
	sigaddset(&mask, INTERRUPT_SIGNAL);
	sigaddset(&mask, EOI_SIGNAL);
	return mask;
}

sigset_t full_mask;

int enable_interrupts() {
	return sigprocmask(SIG_UNBLOCK, &full_mask, NULL);
}

int disable_interrupts() {
	return sigprocmask(SIG_BLOCK, &full_mask, NULL);
}

void handle_signal() {
	ic.handler_address();
}

void accept_signal() {
	disable_interrupts();
	store_context(&proc.stack, proc.current_prio);
	proc.current_prio = ic.current_prio;
	lower_interrupt_flag(&ic, ic.current_prio);
	enable_interrupts();
	handle_signal();
	disable_interrupts();
	proc.current_prio = restore_context(&proc.stack);
	enable_interrupts();
	kill(getpid(), EOI_SIGNAL);
}

REGISTER(1)
REGISTER(2)
REGISTER(3)
REGISTER(4)

int setup_hard_supported() {
	int ret = 0;

	full_mask = get_full_mask();

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

	act.sa_flags = SA_NODEFER;
	act.sa_handler = accept_signal;
	ret |= sigaction(INTERRUPT_SIGNAL, &act, NULL);

	act.sa_handler = finalize_interrupt;
	ret |= sigaction(EOI_SIGNAL, &act, NULL);

	return ret;
}

void loop_hard_supported() {
	while (1) {
		sleep(1);
	}
}

void run_hard_supported() {
	setup_hard_supported();
	loop_hard_supported();
}

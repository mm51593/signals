#include "handlers/ideal_handler.h"
#include "handlers/unsupported_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RADIX 10

int get_arg(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "No arg\n");
		exit(EXIT_FAILURE);
	}

	int arg = strtol(argv[1], NULL, RADIX);

	return arg;
}

void print_pid() {
	int pid = getpid();
	printf("PID: %d\n", pid);
}

void run_by_choice(int choice) {
	switch (choice) {
		case 1:
			run_ideal();
			break;
		case 2:
			run_unsupported();
			break;
		default:
			fprintf(stderr, "Invalid choice\n");
			exit(EXIT_FAILURE);
	}
}


int main(int argc, char *argv[])
{
	print_pid();
	int arg = get_arg(argc, argv);
	run_by_choice(arg);

	return EXIT_SUCCESS;
}

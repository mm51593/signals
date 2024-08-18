#include "handlers/handlers.h"

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

int main(int argc, char *argv[])
{
	print_pid();
	int arg = get_arg(argc, argv);

	switch (arg) {
		case 1:
			if (setup_ideal()) {
				fprintf(stderr, "Setup error\n");
			}	
			break;
		default:
			fprintf(stderr, "Invalid arg: %d\n", arg);
			exit(EXIT_FAILURE);	
	}

	while (1) {
		sleep(1);
	}

	return EXIT_SUCCESS;
}

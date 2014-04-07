#include "cli.h"

void cli_invocation (int argc, char **argv) {
	size_t x;

	fputs ("Chrysalis: invalid invocation:\n\n\t", stderr);
	for (x = 0; x < argc; x++) {
		fputs (argv[x], stderr);
		fputc (' ', stderr);
	}

	fputc ('\n', stderr);
	fputc ('\n', stderr);

	return;
}

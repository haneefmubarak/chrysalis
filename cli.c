#include "cli.h"

void cli_invocation (int argc, char **argv) {
	size_t x;

	fputs ("Chrysalis: invalid invocation:\n\n\t\t", stderr);
	for (x = 0; x < argc; x++) {
		fputs (argv[x], stderr);
		fputc (' ', stderr);
	}

	fputc ('\n', stderr);
	fputc ('\n', stderr);

	fputs (
		"\tcorrect invocation:\n\n\t"
		"name auth [shortflags] action [subaction] [longflags] [-- args]\n\n\t"
		"where:\n\n\t"
		"name:\t\t"		"the name of the command: {cpe, chrysalis}\n\t"
		"auth:\t\t"		"the authorization level and scope of the action: {root, user, local, null}\n\t"
		"[shortflags]:\t"	"optionally, any single letter flags prefixed with a '-'\n\t"
		"action:\t\t"		"the action to perform (try '(name) (auth) -h' for more info)\n\t"
		"[subaction]:\t"	"optionally, a particular subaction to perform (try '(name) (auth) -h (action)' for more info)\n\t"
		"[longflags]:\t"	"optionally, any long flags prefixed with '--'\n\t"
		"[-- args]:\t"		"optionally, '-- ' followed by a list of additional arguments\n\n\t"
		"for more information, try 'man chrysalis'\n\n",
		stderr);

	return;
}

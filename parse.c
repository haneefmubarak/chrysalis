#include "chrysalis.h"

// name:		{chrysalis, cpe, a.out}
// auth:		{root, user, local, null}
// [shortflags]:	{}
// action:		{}
// [subaction]:		{}
// [longflags]...:	{}
// ["--"		{args}*]

struct cli parse_cli (int argc, char **argv) {
	size_t x = 0;

	struct cli cli;

	// check for minimum argc
	if (argc < 4) {
		invocation (argc, argv);
		cli.status = invalid;
		return cli;
	}

	cli.name = parse_name (argv[x++]);
	cli.auth = parse_auth (argv[x++]);
	// validate name and auth
	if ((cli.name == invalid) || (cli.auth == invalid)) {
		invocation (argc, argv);
		cli.status = invalid;
		return cli;
	}

	// check for shortflags
	if (argv[2][0] == '-') {
		cli.sflags = parse_sflags (argv[x++]);

		// validate shortflags
		if (!cli.sflags.sane) {
			invocation (argc, argv);
			cli.status = invalid;
			return cli;
		}
	} else {
		cli.sflags = { 0 };
	}

	cli.action = parse_action (argv[x++]);
	// validate action
	if (cli.action == invalid) {
		invocation (argc, argv);
		cli.status = invalid;
		return cli;
	}

	// check for a subaction
	if (argv[x][0] != '-') {
		cli.subaction = parse_subaction (argv[x++]);

		// validate subaction
		if (cli.subaction == invalid) {
			invocation (argc, argv);
			cli.status = invalid;
			return cli;
		}
	} else {
		cli.subaction = none;
	}

	// parse longflags
	cli.lflags = { 0 };
	// check for null terminator
	while ((x < argc) && (argv[x][2] != 0x00)) {
		// check for first two chars
		if ((argv[x][0] != '-') || (argv[x][1] != '-')) {
			invocation (argc, argv);
			cli.status = invalid;
			return cli;
		)

		// a longflag may take arguments, so
		// we must adjust accordingly
		//
		// also, we need to do this for each
		// flag, and pushing a struct to the
		// stack takes time
		x = parse_lflags (x, argv, &(cli.lflags));
		if (!cli.lflags.sane) {
			invocation (argc, argv);
			cli.status = invalid;
			return cli;
		}
	}

	// if arg is "--" read extra args in
	if ((x < argc) && (strcmp (argv[x++], "--") {
		// enough space for remaining arguments
		cli.argc = argc - x;
		cli.arg = malloc (cli.argc * sizeof (char *));
		assert (cli.arg);
		while (x < argc) {
			cli.arg[x] = strdup (argv[x]);
			assert (cli.arg[x]);
			x++;
		}
	} else {
		cli.argc = 0;
		cli.arg = NULL;
	}

	return cli;
}

static inline enum cli_name parse_name (char *name) {
	enum cli_name label;

	// take advantage of valid IDs list
	switch ((basename (name))[1]) {
		case 'h':	// chrysalis
			if (strcmp (name, "chrysalis"))
				label = invalid;

			label = chrysalis;
			break;

		case 'p'	// cpe
			if (strcmp (name, "cpe"))
				label = invalid;

			label = cpe;
			break;

		case '.'	// a.out
			if (strcmp (name, "a.out"))
				label = invalid;

			label = aout;
			break;

		default:
			label = invalid;
			break;
	}

	return label;
}

static inline enum cli_auth parse_auth (char *auth) {
	enum auth level;

	// take advantage of valid authlevel list
	switch (auth[0]) {
		case 'r':
			if (strcmp (auth, "root"))
				level = invalid;

			level = root;
			break;

		case 'u':
			if (strcmp (auth, "user"))
				level = invalid;

			level = user;
			break;

		case 'l':
			if (strcmp (auth, "local"))
				level = invalid;

			level = local;
			break;

		case 'n':
			if (strcmp (auth, "null"))
				level = invalid;

			level = null;
			break;

		default:
			level = invalid;
			break;
		}

		return level;
}

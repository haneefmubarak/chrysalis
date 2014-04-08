#include "parse.h"
#include "cli.h"

// name:		{chrysalis, cpe, a.out}
// auth:		{root, user, local, null}
// [shortflags]:	{}
// action:		{}
// [subaction]:		{}
// [longflags]...:	{}
// ["--"		{args}*]

static inline enum cli_name parse_name (char *name);
static inline enum cli_auth parse_auth (char *auth);
static inline struct cli_flags parse_sflags (char *sflags);
static inline enum cli_action parse_action (char *action);
static inline struct cli_subaction parse_subaction (char *subaction, enum cli_action action);
static inline size_t parse_lflags (size_t x, char **argv, struct cli_flags *flags);

struct cli parse_cli (int argc, char **argv) {
	size_t x = 0;

	struct cli cli;

	// check for minimum argc
	if (argc < 3) {
		cli_invocation (argc, argv);
		cli.status = status_invalid;
		return cli;
	}

	cli.name = parse_name (argv[x++]);
	cli.auth = parse_auth (argv[x++]);
	// validate name and auth
	if ((cli.name == name_invalid) || (cli.auth == auth_invalid)) {
		cli_invocation (argc, argv);
		cli.status = status_invalid;
		return cli;
	}

	// check for shortflags
	if (argv[2][0] == '-') {
		// detect flags
		cli.flags = parse_sflags (argv[x++]);

		// validate shortflags
		if (cli.flags.insane) {
			cli_invocation (argc, argv);
			cli.status = status_invalid;
			return cli;
		}
	} else {
		cli.flags = (struct cli_flags) { 0 };
	}

	cli.action = parse_action (argv[x++]);
	// validate action
	if (cli.action == action_invalid) {
		cli_invocation (argc, argv);
		cli.status = status_invalid;
		return cli;
	}

	// check for a subaction (and sufficient args)
	if ((x < argc) && (argv[x][0] != '-')) {
		cli.subaction = parse_subaction (argv[x++], cli.action);

		// validate subaction
		if (cli.subaction.insane) {
			cli_invocation (argc, argv);
			cli.status = status_invalid;
			return cli;
		}
	} else {
		cli.subaction = (struct cli_subaction) { 0 };
	}

	// parse longflags
	// check for null terminator
	while ((x < argc) && (argv[x][2] != 0x00)) {
		// check for first two chars
		if ((argv[x][0] != '-') || (argv[x][1] != '-')) {
			cli_invocation (argc, argv);
			cli.status = status_invalid;
			return cli;
		}

		// a longflag may take arguments, so
		// we must adjust accordingly
		//
		// also, we need to do this for each
		// flag, and pushing a struct to the
		// stack takes time
		x = parse_lflags (x, argv, &(cli.flags));
		if (cli.flags.insane) {
			cli_invocation (argc, argv);
			cli.status = status_invalid;
			return cli;
		}
	}

	// if arg is "--" read extra args in
	if ((x < argc) && (strcmp (argv[x++], "--"))) {
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
				label = name_invalid;

			label = name_chrysalis;
			break;

		case 'p':	// cpe
			if (strcmp (name, "cpe"))
				label = name_invalid;

			label = name_cpe;
			break;

		case '.':	// a.out
			if (strcmp (name, "a.out"))
				label = name_invalid;

			label = name_aout;
			break;

		default:
			label = name_invalid;
			break;
	}

	return label;
}

static inline enum cli_auth parse_auth (char *auth) {
	enum cli_auth level;

	// take advantage of valid authlevel list
	switch (auth[0]) {
		case 'r':
			if (strcmp (auth, "root"))
				level = auth_invalid;

			level = auth_root;
			break;

		case 'u':
			if (strcmp (auth, "user"))
				level = auth_invalid;

			level = auth_user;
			break;

		case 'l':
			if (strcmp (auth, "local"))
				level = auth_invalid;

			level = auth_local;
			break;

		case 'n':
			if (strcmp (auth, "null"))
				level = auth_invalid;

			level = auth_null;
			break;

		default:
			level = auth_invalid;
			break;
		}

		return level;
}

static inline struct cli_flags parse_sflags (char *sflags) {
	struct cli_flags flags = { 0 };
	size_t x;

	// ignore the '-' prefix
	for (x = 1; sflags[x] != 0x00; x++) {
		// take advantage of single byte flags
		switch (sflags[x]) {
			case 'v':
				flags.version = 1;
				break;

			default:
				flags.insane = 1;
				break;
		}
	}

	return flags;
}

static inline enum cli_action parse_action (char *action) {
	enum cli_action command;

	// take advantage of valid action list
	switch (action[0]) {
		case 's':
			if (strcmp (action, "status"))
				command = action_invalid;

			command = action_status;
			break;

		case 'u':
			if (strcmp (action, "update"))
				command = action_invalid;

			command = action_update;

		default:
			command = action_invalid;
			break;
		}

		return command;
}

static inline struct cli_subaction parse_subaction (char *subaction, enum cli_action action) {
	struct cli_subaction subcommand = { 0 };

	// take advantage of known subaction list
	switch (subaction[0]) {
		default:
			subcommand.insane = 1;
			break;
	}

	return subcommand;
}

static inline size_t parse_lflags (size_t x, char **argv, struct cli_flags *flags) {
	// take advantage of known longflags
	switch (argv[x][2]) {
		case 'v':
			if (strcmp (argv[x], "--version"))
				flags->insane = 1;

			flags->version = 1;
			break;

		default:
			flags->insane = 1;
			break;
	}

	// increment iterator for next round
	return x+1;
}

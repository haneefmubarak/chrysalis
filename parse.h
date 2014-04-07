#pragma once

#include <libgen.h>

#include "chrysalis.h"

enum cli_name {
	name_invalid,
	name_chrysalis,
	name_cpe,
	name_aout
};

enum cli_auth {
	auth_invalid,
	auth_root,
	auth_user,
	auth_local,
	auth_null
};

enum cli_action {
	action_invalid,
	action_status,
	action_update
};

struct cli_subaction {
	unsigned sane:		1;
};

struct cli_sflags {
	unsigned sane:		1;
	unsigned v:		1;
};

struct cli_lflags {
	unsigned sane:		1;
	unsigned version:	1;
};

struct cli {
	enum status		status;
	enum cli_name		name;
	enum cli_auth		auth;
	enum cli_action		action;
	struct cli_subaction	subaction;
	struct cli_sflags	sflags;
	struct cli_lflags	lflags;
	unsigned		argc;
	char			**arg;
};

struct cli 	parse_cli 		(int argc, char **argv);

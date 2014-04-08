#pragma once

#include <libgen.h>

#include "chrysalis.h"

#define autofree_cli __attribute__((cleanup(cleanup_parse)))

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
	unsigned insane:	1;
};

struct cli_flags {
	unsigned insane:	1;
	unsigned version:	1;
};

struct cli {
	enum status		status;
	enum cli_name		name;
	enum cli_auth		auth;
	enum cli_action		action;
	struct cli_subaction	subaction;
	struct cli_flags	flags;
	unsigned		argc;
	char			**arg;
};

struct cli 	parse_cli 		(int argc, char **argv);
void		cleanup_parse		(void *p);

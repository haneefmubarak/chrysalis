#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef enum {
	root,
	user,
	local,
	null
} chrys_auth_level;

extern chrys_auth_level authLevel;

void		cli_invocation		(int argc, char **argv);

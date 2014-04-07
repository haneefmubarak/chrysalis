#include "chrysalis.h"
#include "parse.h"
#include "cli.h"

int main (int argc, char **argv) {
	struct cli cli = parse_cli (argc, argv);

	return 0;
}

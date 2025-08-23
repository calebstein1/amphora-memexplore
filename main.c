#if defined (__APPLE__) || defined (__linux__)
#include <unistd.h>
#elif defined (_WIN32)
/*
 * TODO: Windows support
 */
#else
#error "unsupported system"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap_handler.h"

static void
print_help(FILE *out) {
	/* It would be strange if this happened ¯\_(ツ)_/¯ */
	if (out != stdout && out != stderr) out = stdout;

	(void)fputs("Usage: memexplore [-d <block>] [-p <block>,<index>] [-l] [-r] [-h]\n", out);
}

static int
validate_blk_idx(const char *s) {
	char *p;

	p = strchr(s, ',');
	if (p == NULL) return 0;

	return (int)(p - s) > 0 && (int)(p - s) < (int)strlen(s) - 1;
}

int
main(int argc, char *argv[]) {
	int c;

	if (open_heap() != 0) return 1;
	while ((c = getopt(argc, argv, "d:p:lrh")) != -1) {
		switch (c) {
			case 'd':
				dump_block((int)strtol(optarg, NULL, 0));
				break;
			case 'p':
				if (validate_blk_idx(optarg) == 0) {
					(void)fputs("Invalid argument for -p, requires -p <block>,<index>\n", stderr);
					close_heap();

					return 1;
				}
				peek_addr((int)strtol(strtok(optarg, ","), NULL, 0), (int)strtol(strtok(NULL, ""), NULL, 0));
				break;
			case 'l':
				list_categories();
				break;
			case 'r':
				reset_heap();
				break;
			case 'h':
				print_help(stdout);
				break;
			case '?':
			default:
				print_help(stderr);
				close_heap();

				return 1;
		}
	}
	close_heap();

	return 0;
}

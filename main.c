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

int
main(int argc, char *argv[]) {
	int c, n;
	char *p;

	if (open_heap() != 0) return 1;
	while ((c = getopt(argc, argv, "d:p:l")) != -1) {
		switch (c) {
			case 'd':
				dump_block((int)strtol(optarg, NULL, 0));
				break;
			case 'p':
				p = strchr(optarg, ',');
				if (p == NULL) goto comma_fail;
				n = (int)(p - optarg);
				if (n <= 0 || n >= strlen(optarg) - 1) {
					comma_fail:
					(void)fputs("Invalid argument for -p, requires -p <block,index>\n", stderr);
					close_heap();

					return 1;
				}
				peek_addr((int)strtol(strtok(optarg, ","), NULL, 0), (int)strtol(strtok(NULL, ""), NULL, 0));
				break;
			case 'l':
				list_categories();
				break;
			case '?':
			default:
				(void)fputs("Usage: memexplore [-d <block>] [-p <block,index>] [-l]\n", stderr);
		}
	}
	close_heap();

	return 0;
}

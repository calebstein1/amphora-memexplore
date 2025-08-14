#if defined(__APPLE__) || defined(__linux__)
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#elif defined(_WIN32)
#include <memoryapi.h>
#else
#error "unsupported system"
#endif

#include <stdio.h>

#include "commands.h"
#include "memexplore.h"

#include <stdlib.h>
#include <string.h>

AmphoraMemBlock *amphora_heap;
struct amphora_mem_block_metadata_t *heap_metadata;
const char *category_names[] = {
#define X(cat) #cat,
	AMPHORA_MEM_CATEGORIES
#undef X
};
const char *cmd_list[] = {
#define X(id, name) name,
	COMMAND_LIST
#undef X
};

int
main(int argc, char *argv[]) {
	int fd;
	char in_buffer[1024];
	char *cmd;
	char *args;
	MemExploreCommand cmd_id;
	int c;

	fd = shm_open("/amphora_heap", O_RDONLY, 0666);
	if (fd == -1) {
		(void)fputs("Failed to connect to Amphora instance\n", stderr);
		return -1;
	}
	amphora_heap = mmap(NULL, sizeof(AmphoraMemBlock) * AMPHORA_NUM_MEM_BLOCKS, PROT_READ, MAP_SHARED, fd, 0);
	(void)close(fd);
	if (amphora_heap == NULL) return -1;

	heap_metadata = (struct amphora_mem_block_metadata_t *)&amphora_heap[AMPHORA_NUM_MEM_BLOCKS - 1][8];
	while ((c = getopt(argc, argv, "d:p:l")) != -1) {
		switch (c) {
			case 'd':
				dump_block(atoi(optarg));
				break;
			case 'p':
				peek_addr(atoi(optarg), atoi(optarg + 1));
				break;
			case 'l':
				list_categories();
				break;
			case '?':
			default:
				fputs("Usage: memexplore [-d <block>] [-p <block> <index>] [-l]\n", stderr);
		}
	}

	(void)munmap(amphora_heap, sizeof(AmphoraMemBlock) * AMPHORA_NUM_MEM_BLOCKS);
	return 0;
}

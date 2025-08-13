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

#include "amphora_memexplore.h"

#include <stdlib.h>
#include <string.h>

static AmphoraMemBlock *amphora_heap;
static struct amphora_mem_block_metadata_t *heap_metadata;
static const char *category_names[] = {
#define X(cat) #cat,
	AMPHORA_MEM_CATEGORIES
#undef X
};
static const char *cmd_list[] = {
#define X(id, name) name,
	COMMAND_LIST
#undef X
};

MemExploreCommand
parse_cmd(const char *cmd) {
	MemExploreCommand cmd_id;

	for (cmd_id = 0; cmd_id < CMD_COUNT; cmd_id++) {
		if (strcmp(cmd, cmd_list[cmd_id]) == 0) {
			return cmd_id;
		}
	}

	return -1;
}

void
dump_block(const char *args) {
	int addr;
	int i;

	if (args == NULL) {
		(void)puts("No block supplied");
		return;
	}
	addr = atoi(args);
	if (addr >= AMPHORA_NUM_MEM_BLOCKS) {
		(void)printf("Invalid memory block address: %d\n", addr);
		return;
	}
	(void)printf("Memory block %d:\nCategory: %s\nAllocations: %d", addr, category_names[heap_metadata[addr].category], heap_metadata[addr].allocations);
	for (i = 0; i < sizeof(AmphoraMemBlock); i++) {
		if ((i & 15) == 0) (void)printf("\n%5d: ", i);
		(void)printf("%02X ", amphora_heap[addr][i]);
	}
	(void)fputs("\n", stdout);
}

void
peek_addr(const char *args) {
	int block, idx;

	if (args == NULL) {
		(void)fputs("No block supplied\n", stderr);
		return;
	}
	block = atoi(strtok(args, " \n"));
	if (block >= AMPHORA_NUM_MEM_BLOCKS) {
		(void)fprintf(stderr, "Invalid memory block address: %d\n", block);
		return;
	}
	idx = atoi(strtok(NULL, "\n"));
	if (idx >= sizeof(AmphoraMemBlock)) {
		(void)fprintf(stderr, "Invalid memory block index: %d\n", idx);
		return;
	}
	(void)printf("%02X\n", amphora_heap[block][idx]);
}

void
list_categories(void) {
	int i;

	for (i = 0; i < AMPHORA_NUM_MEM_BLOCKS; i++) {
		(void)printf("%3d: %s\n", i, category_names[heap_metadata[i].category]);
	}
}

int
main(void) {
	int fd;
	char in_buffer[1024];
	char *cmd;
	char *args;
	MemExploreCommand cmd_id;

	fd = shm_open("/amphora_heap", O_RDONLY, 0666);
	if (fd == -1) {
		fputs("Failed to connect to Amphora instance\n", stderr);
		return -1;
	}
	amphora_heap = mmap(NULL, sizeof(AmphoraMemBlock) * AMPHORA_NUM_MEM_BLOCKS, PROT_READ, MAP_SHARED, fd, 0);
	(void)close(fd);
	if (amphora_heap == NULL) return -1;

	heap_metadata = (struct amphora_mem_block_metadata_t *)&amphora_heap[AMPHORA_NUM_MEM_BLOCKS - 1][8];

	(void)puts("Connected to Amphora instance!");
	while (1) {
		(void)printf("> ");
		(void)fgets(in_buffer, sizeof(in_buffer), stdin);
		if (strlen(in_buffer) == 1) continue;
		cmd = strtok(in_buffer, " \n");
		args = strtok(NULL, "\n");
		cmd_id = parse_cmd(cmd);
		switch (cmd_id) {
			case CMD_DUMP:
				dump_block(args);
				break;
			case CMD_PEEK:
				peek_addr(args);
				break;
			case CMD_LIST:
				list_categories();
				break;
			case CMD_CLS:
				fputs("\033[2J\033[H", stdout);
				break;
			case CMD_EXIT:
				goto leave;
			default:
				(void)printf("Unknown command: %s\n", cmd);
		}
	}

	leave:
	(void)munmap(amphora_heap, sizeof(AmphoraMemBlock) * AMPHORA_NUM_MEM_BLOCKS);
	return 0;
}

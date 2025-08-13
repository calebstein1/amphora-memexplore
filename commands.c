#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

extern AmphoraMemBlock *amphora_heap;
extern struct amphora_mem_block_metadata_t *heap_metadata;
extern const char *category_names[];
extern const char *cmd_list[];

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
peek_addr(char *args) {
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

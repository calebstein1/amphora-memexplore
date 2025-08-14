#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

extern AmphoraMemBlock *amphora_heap;
extern struct amphora_mem_block_metadata_t *heap_metadata;
extern const char *category_names[];
extern const char *cmd_list[];

void
dump_block(int blk) {
	int i;

	if (blk >= AMPHORA_NUM_MEM_BLOCKS) {
		(void)fprintf(stderr, "Invalid memory block address: %d\n", blk);
		return;
	}
	(void)printf("Memory block %d:\nCategory: %s\nAllocations: %d", blk, category_names[heap_metadata[blk].category], heap_metadata[blk].allocations);
	for (i = 0; i < sizeof(AmphoraMemBlock); i++) {
		if ((i & 15) == 0) (void)printf("\n%5d: ", i);
		(void)printf("%02X ", amphora_heap[blk][i]);
	}
	(void)fputs("\n", stdout);
}

void
peek_addr(int blk, int idx) {
	if (blk >= AMPHORA_NUM_MEM_BLOCKS) {
		(void)fprintf(stderr, "Invalid memory block address: %d\n", blk);
		return;
	}
	if (idx >= sizeof(AmphoraMemBlock)) {
		(void)fprintf(stderr, "Invalid memory block index: %d\n", idx);
		return;
	}
	(void)printf("%02X\n", amphora_heap[blk][idx]);
}

void
list_categories(void) {
	int i;

	for (i = 0; i < AMPHORA_NUM_MEM_BLOCKS; i++) {
		(void)printf("%3d: %s\n", i, category_names[heap_metadata[i].category]);
	}
}

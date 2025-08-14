#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

extern AmphoraMemBlock *amphora_heap;
extern struct amphora_mem_block_metadata_t *heap_metadata;
extern const char *category_names[];
extern const char *cmd_list[];

#define PRINTABLE_CHAR(c) ((c >= 0x20 && c <= 0x7E) ? c : '.')

void
dump_block(int blk) {
	int i;

	if (blk >= AMPHORA_NUM_MEM_BLOCKS) {
		(void)fprintf(stderr, "Invalid memory block address: %d\n", blk);
		return;
	}
	(void)printf("Memory block %d:\nCategory: %s\nAllocations: %d", blk, category_names[heap_metadata[blk].category], heap_metadata[blk].allocations);
	for (i = 0; i < sizeof(AmphoraMemBlock); i += 16) {
		(void)printf("\n%04X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
			i,
			amphora_heap[blk][i],
			amphora_heap[blk][i + 1],
			amphora_heap[blk][i + 2],
			amphora_heap[blk][i + 3],
			amphora_heap[blk][i + 4],
			amphora_heap[blk][i + 5],
			amphora_heap[blk][i + 6],
			amphora_heap[blk][i + 7],
			amphora_heap[blk][i + 8],
			amphora_heap[blk][i + 9],
			amphora_heap[blk][i + 10],
			amphora_heap[blk][i + 11],
			amphora_heap[blk][i + 12],
			amphora_heap[blk][i + 13],
			amphora_heap[blk][i + 14],
			amphora_heap[blk][i + 15],
			PRINTABLE_CHAR(amphora_heap[blk][i]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 1]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 2]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 3]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 4]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 5]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 6]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 7]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 8]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 9]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 10]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 11]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 12]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 13]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 14]),
			PRINTABLE_CHAR(amphora_heap[blk][i + 15])
		);
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

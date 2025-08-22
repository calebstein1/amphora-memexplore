#include <string.h>
#if defined(__APPLE__) || defined(__linux__)
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#elif defined(_WIN32)
#include <memoryapi.h>
/*
 * TODO: Windows support
 */
#else
#error "unsupported system"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "amphora_mem.h"
#include "colors.h"
#include "heap_handler.h"

static AmphoraMemBlock *amphora_heap;
static struct amphora_mem_block_metadata_t *heap_metadata;
const char *category_names[] = {
#define X(cat) #cat,
	AMPHORA_MEM_CATEGORIES
#undef X
};


#define PRINTABLE_CHAR(c) ((c >= 0x20 && c <= 0x7E) ? c : '.')

int
open_heap(void) {
	int fd;

	fd = shm_open("/amphora_heap", O_RDONLY, 0666);
	if (fd == -1) {
		(void)fputs("Failed to connect to Amphora instance\n", stderr);
		return -1;
	}
	amphora_heap = mmap(NULL, sizeof(AmphoraMemBlock) * AMPHORA_NUM_MEM_BLOCKS, PROT_READ, MAP_SHARED, fd, 0);
	(void)close(fd);
	if (amphora_heap == NULL) return -1;

	heap_metadata = (struct amphora_mem_block_metadata_t *)&amphora_heap[AMPHORA_NUM_MEM_BLOCKS - 1][8];

	return 0;
}

void
close_heap(void) {
	/* Can safely be ignored since this code path will never be reached with an invalid address, and we're leaving anyway */
	(void)munmap(amphora_heap, sizeof(AmphoraMemBlock) * AMPHORA_NUM_MEM_BLOCKS);
}

void
reset_heap(void) {
	(void)shm_unlink("/amphora_heap");
}

void
dump_block(unsigned int blk) {
	unsigned int i;
	char s[3][8];
	struct amphora_mem_allocation_header_t *h1, *h2;

	if (blk >= AMPHORA_NUM_MEM_BLOCKS) {
		(void)fprintf(stderr, "Invalid memory block address: %d\n", blk);
		return;
	}
	(void)printf("Memory block %d:\nCategory: %s\nAllocations: %d\nCorrupted: %s", blk, category_names[heap_metadata[blk].category], heap_metadata[blk].allocations, heap_metadata[blk].corrupted ? "yes" : "no");
	for (i = 0; i < sizeof(AmphoraMemBlock); i += 16) {
		h1 = (struct amphora_mem_allocation_header_t *)&amphora_heap[blk][i];
		h2 = (struct amphora_mem_allocation_header_t *)&amphora_heap[blk][i + 8];
		strcpy(s[0], h1->magic == MAGIC ? h1->free ? PURPLE : GREEN : RESET);
		strcpy(s[1], h2->magic == MAGIC ? h2->free ? PURPLE : GREEN : RESET);
		strcpy(s[2], RESET);
		if (h1->magic != MAGIC && h2->magic != MAGIC) memset(s, 0, sizeof(s));
		(void)printf("\n%04X: %s%02X %02X %02X %02X %02X %02X %02X %02X %s%02X %02X %02X %02X %02X %02X %02X %02X%s\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
			i,
			s[0],
			amphora_heap[blk][i],
			amphora_heap[blk][i + 1],
			amphora_heap[blk][i + 2],
			amphora_heap[blk][i + 3],
			amphora_heap[blk][i + 4],
			amphora_heap[blk][i + 5],
			amphora_heap[blk][i + 6],
			amphora_heap[blk][i + 7],
			s[1],
			amphora_heap[blk][i + 8],
			amphora_heap[blk][i + 9],
			amphora_heap[blk][i + 10],
			amphora_heap[blk][i + 11],
			amphora_heap[blk][i + 12],
			amphora_heap[blk][i + 13],
			amphora_heap[blk][i + 14],
			amphora_heap[blk][i + 15],
			s[2],
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
peek_addr(unsigned int blk, unsigned int idx) {
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
	int i, corrupt;

	for (i = 0; i < AMPHORA_NUM_MEM_BLOCKS; i++) {
		corrupt = heap_metadata[i].corrupted;
		(void)printf("%3d:\t%s\t%s%s%s\n", i, category_names[heap_metadata[i].category], corrupt ? RED : GREEN , corrupt ? "CORRUPTED" : "OK", RESET);
	}
}

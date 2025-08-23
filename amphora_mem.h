#ifndef AMPHORA_MEMEXPLORE_H
#define AMPHORA_MEMEXPLORE_H

#define MAGIC 0xEFBE

#define AMPHORA_MEM_CATEGORIES	\
	X(MEM_UNASSIGNED)	\
	X(MEM_RENDERABLE)	\
	X(MEM_IMAGE)		\
	X(MEM_EMITTER)		\
	X(MEM_TILEMAPS)		\
	X(MEM_STRING)		\
	X(MEM_HASHTABLE)	\
	X(MEM_MISC)		\
	X(MEM_META)

#define AMPHORA_HEAP_SIZE 0x10000
#define AMPHORA_NUM_MEM_BLOCKS 0x100

typedef unsigned char AmphoraMemBlock[AMPHORA_HEAP_SIZE];

typedef enum {
#define X(cat) cat,
	AMPHORA_MEM_CATEGORIES
	MEM_COUNT
#undef X
} AmphoraMemBlockCategory;

struct amphora_mem_block_metadata_t {
	unsigned short largest_free;
	unsigned short allocations;
	AmphoraMemBlockCategory category;
	unsigned char corrupted : 1;
	unsigned char recovery_count : 3;
};

struct amphora_mem_allocation_header_t {
	unsigned short magic;
	unsigned char scope;
	unsigned char free : 1;
	unsigned char large : 1;
	unsigned short off_b;
	unsigned short off_f;
};

#endif /* AMPHORA_MEMEXPLORE_H */

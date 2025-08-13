#ifndef AMPHORA_MEMEXPLORE_H
#define AMPHORA_MEMEXPLORE_H

#include <stdint.h>

#define AMPHORA_MEM_CATEGORIES	\
	X(MEM_UNASSIGNED)	\
	X(MEM_RENDERABLE)	\
	X(MEM_IMAGE)		\
	X(MEM_EMITTER)		\
	X(MEM_TILEMAPS)		\
	X(MEM_STRING)		\
	X(MEM_HASHTABLE)	\
	X(MEM_MISC)		\
	X(MEM_META)		\
	X(MEM_COUNT)

#define COMMAND_LIST			\
	X(CMD_DUMP, "dump")		\
	X(CMD_PEEK, "peek")		\
	X(CMD_LIST, "list_categories")	\
	X(CMD_CLS, "clear")		\
	X(CMD_EXIT, "exit")		\
	X(CMD_COUNT, "")

#define AMPHORA_HEAP_SIZE 0x10000
#define AMPHORA_NUM_MEM_BLOCKS 0x100

typedef uint8_t AmphoraMemBlock[AMPHORA_HEAP_SIZE];

typedef enum {
#define X(cat) cat,
	AMPHORA_MEM_CATEGORIES
#undef X
} AmphoraMemBlockCategory;

typedef enum {
#define X(id, name) id,
	COMMAND_LIST
#undef X
} MemExploreCommand;

struct amphora_mem_block_metadata_t {
	uint16_t addr;
	uint16_t allocations;
	AmphoraMemBlockCategory category;
};

#endif /* AMPHORA_MEMEXPLORE_H */

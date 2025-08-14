#ifndef COMMANDS_H
#define COMMANDS_H

#include "memexplore.h"

MemExploreCommand parse_cmd(const char *cmd);
void dump_block(int);
void peek_addr(int, int);
void list_categories(void);

#endif /* COMMANDS_H */

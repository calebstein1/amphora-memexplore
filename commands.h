#ifndef COMMANDS_H
#define COMMANDS_H

#include "memexplore.h"

MemExploreCommand parse_cmd(const char *cmd);
void dump_block(const char *args);
void peek_addr(char *args);
void list_categories(void);

#endif /* COMMANDS_H */

#ifndef COMMANDS_H
#define COMMANDS_H

int open_heap(void);
void close_heap(void);
void reset_heap(void);
void dump_block(unsigned int);
void peek_addr(unsigned int, unsigned int);
void list_categories(void);

#endif /* COMMANDS_H */

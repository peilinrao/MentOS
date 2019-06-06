
#ifndef _TER_H
#define _TER_H

#include "types.h"

#define MAX_TERM 3
#define VIDMEM_SIZE 4096

extern uint32_t current_terminal;

void switch_term(int32_t term);

#endif /* _TER_H */

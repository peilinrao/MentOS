
#ifndef _IDT_JMP_H
#define _IDT_JMP_H

#include "types.h"

#ifndef ASM
/**
  DESCRIPTION: IDT jump table. Used to push argument and call do_irq
*/
extern uint32_t IDT_JMP_TBL; // creating header for jump table
extern uint32_t IRQ_ASM_LNK; // header for assembly linkage

#endif /* ASM */

#endif /* _IDT_JMP_H */

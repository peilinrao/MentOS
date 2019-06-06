
#ifndef _PIT_H
#define _PIT_H

#include "types.h"
#include "syscall.h"

#define PIT_CTL_PORT 0x43
#define PIT_CH0_PORT 0x40
#define PIT_CTL_WORD 0x36

#define PIT_IRQ_NUM 0x20

#define LAST_PCB 0x7f0000
#define SOME_NUMBER 20

typedef struct stack_context_2{
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	int32_t ebx;
	int32_t edx;
	int32_t ecx;
	int32_t eax;
	uint32_t retaddr;
	uint32_t cs;
	uint32_t flags;
	uint32_t esp_old;
	uint32_t ss;
}stack_context_2_t;

void pit_irq(void);
void pit_init(void);

extern uint32_t switch_process;
extern uint32_t syshlt;
extern char sys_halted;

#endif /* _PIT_H */

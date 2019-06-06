
#ifndef _IRQ_H
#define _IRQ_H

#include "types.h"


#define EXC_DIV_BY_ZERO   0x00
#define EXC_DEBUG         0x01
#define EXC_NMI           0x02
#define EXC_BREAKPOINT    0x03
#define EXC_OVERFLOW      0x04
#define EXC_ROUND_EXC     0x05
#define EXC_INV_OP        0x06
#define EXC_DEV_NOT_AVAIL 0x07
#define EXC_DOUBLE_FAULT  0x08
#define EXC_CSO           0x09
#define EXC_INV_TSS       0x0A
#define EXC_SNP           0x0B
#define EXC_SSF           0x0C
#define EXC_GPF           0x0D
#define EXC_PAGE_FAULT    0x0E
#define EXC_FLOATING      0x10
#define EXC_ALIGN_CHECK   0x11
#define EXC_MACH_CHECK    0x12
#define EXC_SIMD          0x13
#define EXC_VIS_EXC       0x14
#define EXC_SEC_EXC       0x1E
#define SYS_CALL          0x80

#define IRQ_START         0x20
#define IRQ_SLAVE_START   0x28
#define IRQ_END           0x2F

typedef struct stack_irq{
	uint32_t irq_num;
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
}stack_irq_t;

typedef struct stack_err{
	uint32_t irq_num;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	int32_t ebx;
	int32_t edx;
	int32_t ecx;
	int32_t eax;
	uint32_t errcode;
	uint32_t retaddr;
	uint32_t cs;
	uint32_t flags;
	uint32_t esp_old;
	uint32_t ss;
	uint32_t stack_content[4];
}stack_err_t;

/* called upon each irq */
void do_irq(int32_t irq_num);

#endif /* _IRQ_H */

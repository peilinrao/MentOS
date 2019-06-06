#include "types.h"
#include "irq.h"
#include "lib.h"
#include "i8259.h"
#include "kb.h"
#include "rtc.h"
#include "pit.h"
#include "syscall.h"

/* halt:
		alt to while(1)
		INPUT: none
		OUTPUT: none
*/
void irq_halt(){
	tprintf("Halted due to error\n");
	asm volatile (".2: hlt; jmp .2;");
}

/* get_cr2:
		Helper function to get cr2
		INPUT: none
		OUTPUT: none
*/
static inline uint32_t get_cr2() {
    uint32_t val;
    asm volatile ("             \n\
            movl  %%cr2, %0     \n\
            "
            : "=a"(val)
            :
            : "memory", "cc"
    );
    return val;
}

/* do_irq:
		DESCRIPTION: call irq handler according to irq number
		INPUT: irq_num: interrupt request number
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: call handlers
*/
void do_irq(int32_t irq_num){
	//The negative irq numbers were pushed on the stack, now take inverse of that
	irq_num = ~irq_num;
	//Setting different interrupt handlers
	stack_irq_t* stack = (stack_irq_t*)(&irq_num);
	stack_err_t* stack_err = (stack_err_t*)(&irq_num);
	switch (irq_num){
		case EXC_DIV_BY_ZERO:     tprintf("\nException: divison by zero!\n");               irq_halt();
		case EXC_DEBUG:           tprintf("\nException: debug!\n");                         irq_halt();
		case EXC_NMI:             tprintf("\nException: non-maskable interrupt!\n");        irq_halt();
		case EXC_BREAKPOINT:      tprintf("\nException: breakpoint!\n");                    irq_halt();
		case EXC_OVERFLOW:        tprintf("\nException: overflow!\n");                      irq_halt();
		case EXC_ROUND_EXC:       tprintf("\nException: bound range Exception!\n");         irq_halt();
		case EXC_INV_OP:          tprintf("\nException: invalid operation!\n");             irq_halt();
		case EXC_DEV_NOT_AVAIL:   tprintf("\nException: device not available!\n");          irq_halt();
		case EXC_DOUBLE_FAULT:    tprintf("\nException: double fault!\n");                  irq_halt();
		case EXC_INV_TSS:         tprintf("\nException: invalid TSS!\n");                   irq_halt();
		case EXC_SNP:             tprintf("\nException: segmentation not present!\n");      irq_halt();
		case EXC_SSF:             tprintf("\nException: stack-segment fault!\n");           irq_halt();
		case EXC_GPF:             tprintf("\nException: general protection fault!\n");      irq_halt();
			case EXC_PAGE_FAULT:{
									tprintf("\nException: page fault!\n");
									tprintf("CR2 Content: %x\n", get_cr2());
									tprintf("Error Code: %x\n", stack_err->errcode);
									tprintf("Offending EIP: %x\n", stack_err->retaddr);
									tprintf("EAX: %x, EBX: %x, ECX: %x, EDX: %x\n", stack_err->eax, stack_err->ebx, stack_err->ecx, stack_err->edx);
									tprintf("ESP: %x, EBP: %x, ESI: %x, EDI: %x\n", stack_err->esp, stack_err->ebp, stack_err->esi, stack_err->edi);
									tprintf("Stack Content:\n");
									int i;
									for(i = 0; i < 4; i++){
										tprintf("%dth: %x\n", i, stack_err->stack_content[i]);
									}
									irq_halt();
								}
		case EXC_FLOATING:        tprintf("\nException: x87 floating-point exception!\n");  irq_halt();
		case EXC_ALIGN_CHECK:     tprintf("\nException: alignment check!\n");               irq_halt();
		case EXC_MACH_CHECK:      tprintf("\nException: machine check!\n");                 irq_halt();
		case EXC_SIMD:            tprintf("\nException: SIMD floating-point exception!\n"); irq_halt();
		case EXC_VIS_EXC:         tprintf("\nException: virtualization exception!\n");      irq_halt();
		case EXC_SEC_EXC:         tprintf("\nException: security exception!\n");            irq_halt();
		case SYS_CALL:			{
									stack->eax = syscall(stack->eax, (void*)stack->ebx, (void*)stack->ecx, (void*)stack->edx);
									break;
								}

		case KB_IRQ_NUM: kb_irq(); break;
		case RTC_IRQ_NUM: rtc_irq(); break;
		case PIT_IRQ_NUM: pit_irq(); break;
		default: break;
	}
	//Send eoi
	if(irq_num >= IRQ_START && irq_num <= IRQ_END) send_eoi(irq_num);
	if(irq_num >= IRQ_SLAVE_START && irq_num <= IRQ_END) send_eoi(2);
}

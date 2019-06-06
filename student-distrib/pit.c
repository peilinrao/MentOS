#include "types.h"
#include "lib.h"
#include "i8259.h"
#include "pit.h"
#include "syscall.h"
#include "x86_desc.h"
#include "kb.h"
#include "rtc.h"

pcb_addr_finder_t* junk;
int32_t itr;
char sys_halted = 0;


/* pit_irq:
		DESCRIPTION: It hands the pit interrupt
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: NONE
*/
void pit_irq(){
	asm volatile (".globl switch_process");
	asm volatile ("switch_process:");
	
	asm volatile ("pushal");
	if(!sys_halted){
		asm volatile ("movl %%esp, %%eax			\n\
						movl %%ebp, %%ebx"
						: "=a"(current_pcb->saved_esp), "=b"(current_pcb->saved_ebp)
						:
						: "memory", "cc"
		);
	}
	asm volatile (".globl new_process2");
	asm volatile ("new_process2:");
	
	itr = 0;
	junk = (pcb_addr_finder_t*)(current_pcb);
	while(1){
		junk--;
		itr++;
		if(itr > SOME_NUMBER) asm volatile ("jmp sys_halt");
		if((uint32_t)junk < LAST_PCB) junk+=PROGRAM_COUNT;
		if(!junk->pcb.valid) continue;
		if(junk->pcb.scheduled) break;
	}
	sys_halted = 0;
	current_pcb = &(junk->pcb);
	
	asm volatile ("movl %0, %%cr3"
					:
					: "a"(page_dir[(uint32_t)(current_pcb->this_pid)])
					: "memory", "cc"
	);
	
	asm volatile ("movl %%eax, %%esp			\n\
					movl %%ebx, %%ebp"
					: 
					: "a"(current_pcb->saved_esp), "b"(current_pcb->saved_ebp)
					: "memory", "cc"
	);
	tss.esp0 = (uint32_t)junk + PCB_TO_STACK;
	send_eoi(0);
	asm volatile ("popal");
	asm volatile ("iret");
	
	asm volatile ("sys_halt:");
	//                   ↓ trash can ESP
	asm volatile ("movl $0x7efffc, %esp");
	sys_halted = 1;
	send_eoi(0);
	asm volatile ("sti");
	asm volatile (".globl syshlt");
	asm volatile (".syshlt: hlt; jmp .syshlt;");
}

/* pit_init:
		DESCRIPTION: Enables pit interrupt
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: set frequency and enable irq
*/
void pit_init(){
	outb(PIT_CTL_WORD, PIT_CTL_PORT);
	outb(0x00, PIT_CH0_PORT);
	outb(0x40, PIT_CH0_PORT);
	enable_irq(0);
}

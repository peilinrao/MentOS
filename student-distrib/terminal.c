#include "terminal.h"
#include "syscall.h"
#include "lib.h"

uint32_t current_terminal = 0;

static char* real_vmem = (char*)VIDEO;
static char* vmem_map[MAX_TERM] = {(char*)VIDEO1, (char*)VIDEO2, (char*)VIDEO3};


/* switch_term:
		DESCRIPTION: handle the switching between terms
		INPUT: term: the terminal we want to switch to
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: NONE
*/
void switch_term(int32_t term){
	int i;
	if(current_terminal != term){
		memcpy(vmem_map[current_terminal], real_vmem, VIDMEM_SIZE);
		current_terminal = term;
		memcpy(real_vmem, vmem_map[current_terminal], VIDMEM_SIZE);
		tupdate_cursor();
	}
	pcb_addr_finder_t* pcb_ptr = (pcb_addr_finder_t*)(PCB_FDR);
	for(i = 0; i < PROGRAM_COUNT; i++, pcb_ptr--){
		if(pcb_ptr->pcb.valid){
			if(pcb_ptr->pcb.terminal_num == current_terminal){
				page_tbl2[i][0].page_table_base_address = (uint32_t)real_vmem >> PAGETBL_OFFSET;
			} else {
				page_tbl2[i][0].page_table_base_address = (uint32_t)vmem_map[pcb_ptr->pcb.terminal_num] >> PAGETBL_OFFSET;
			}
		} else {
			page_tbl2[i][0].page_table_base_address = VIDEO4 >> PAGETBL_OFFSET;
		}
	}
	asm volatile ("movl %0, %%cr3"
					:
					: "a"(page_dir[(uint32_t)(current_pcb->this_pid)])
					: "memory", "cc"
	);
}

#include "page.h"
#include "lib.h"

/* declare the page directory */
//4096: 4k per page
pde_t __attribute__((aligned(4096))) page_dir[PROGRAM_COUNT][PGGE_KB_SIZE] = {{{0}}};

/* declare the page table */
pte_t __attribute__((aligned(4096))) page_tbl1[PROGRAM_COUNT][PGGE_KB_SIZE] = {{{0}}};
pte_t __attribute__((aligned(4096))) page_tbl2[PROGRAM_COUNT][PGGE_KB_SIZE] = {{{0}}};

void page_init(){
	/*Paging starts*/
	int i;
	for(i = 0; i < PROGRAM_COUNT; i++){
		// directory entry 0 (physical memory 0-4MB)
		// aligned to page table. (for video momory)
		page_dir[i][0].present = 1;
		page_dir[i][0].read_write = 1;
		page_dir[i][0].user_supervisor = 0;
		page_dir[i][0].page_size = 0;
		page_dir[i][0].page_table_base_address = (uint32_t)page_tbl1[i] >> PAGETBL_OFFSET;
		
		// directory entry 1 (physical memory 4-8MB)
		// extanded page
		page_dir[i][1].present = 1;
		page_dir[i][1].read_write = 1;
		page_dir[i][1].user_supervisor = 0;
		page_dir[i][1].page_size = 1;
		page_dir[i][1].page_table_base_address = KERN >> PAGETBL_OFFSET;
		
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET)].present = 1;
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET)].read_write = 1;
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET)].user_supervisor = 1;
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET)].page_size = 1;
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET)].page_table_base_address = (PROGRAM_START + PROGRAM_SIZE*i) >> PAGETBL_OFFSET;
		
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET) + 1].present = 1;
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET) + 1].read_write = 1;
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET) + 1].user_supervisor = 0;
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET) + 1].page_size = 0;
		page_dir[i][(PROGRAM_START >> PAGEDIR_OFFSET) + 1].page_table_base_address = (uint32_t)page_tbl2[i] >> PAGETBL_OFFSET;
		
		
		// set video memory page entry
		page_tbl1[i][VIDEO >> PAGETBL_OFFSET].present = 1;
		page_tbl1[i][VIDEO >> PAGETBL_OFFSET].read_write = 1;
		page_tbl1[i][VIDEO >> PAGETBL_OFFSET].user_supervisor = 0;
		page_tbl1[i][VIDEO >> PAGETBL_OFFSET].page_table_base_address = VIDEO >> PAGETBL_OFFSET;
		
		page_tbl1[i][VIDEO1 >> PAGETBL_OFFSET].present = 1;
		page_tbl1[i][VIDEO1 >> PAGETBL_OFFSET].read_write = 1;
		page_tbl1[i][VIDEO1 >> PAGETBL_OFFSET].user_supervisor = 0;
		page_tbl1[i][VIDEO1 >> PAGETBL_OFFSET].page_table_base_address = VIDEO1 >> PAGETBL_OFFSET;
		
		page_tbl1[i][VIDEO2 >> PAGETBL_OFFSET].present = 1;
		page_tbl1[i][VIDEO2 >> PAGETBL_OFFSET].read_write = 1;
		page_tbl1[i][VIDEO2 >> PAGETBL_OFFSET].user_supervisor = 0;
		page_tbl1[i][VIDEO2 >> PAGETBL_OFFSET].page_table_base_address = VIDEO2 >> PAGETBL_OFFSET;
		
		page_tbl1[i][VIDEO3 >> PAGETBL_OFFSET].present = 1;
		page_tbl1[i][VIDEO3 >> PAGETBL_OFFSET].read_write = 1;
		page_tbl1[i][VIDEO3 >> PAGETBL_OFFSET].user_supervisor = 0;
		page_tbl1[i][VIDEO3 >> PAGETBL_OFFSET].page_table_base_address = VIDEO3 >> PAGETBL_OFFSET;
		
		page_tbl2[i][0].present = 1;
		page_tbl2[i][0].read_write = 1;
		page_tbl2[i][0].user_supervisor = 0;
		page_tbl2[i][0].page_table_base_address = VIDEO4 >> PAGETBL_OFFSET;
		
	}
	
	// set up control registers to initialize paging
	asm volatile ("movl %0, %%cr3					\n\
					movl %%cr4, %%eax				\n\
					or   $0x00000010, %%eax			\n\
					movl %%eax, %%cr4				\n\
					movl %%cr0, %%eax				\n\
					or   $0x80000001, %%eax			\n\
					movl %%eax, %%cr0"				\
					:								\
					: "a"(page_dir[0])				\
					: "memory", "cc"				\
	);
}

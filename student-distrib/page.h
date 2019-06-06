
#ifndef _PAGE_H
#define _PAGE_H

#define PAGETBL_OFFSET 12
#define PAGEDIR_OFFSET 22
#define PGGE_KB_SIZE 1024

#include "types.h"

/* struct for page directory entry */
typedef struct __attribute__((packed)) pde{
	uint32_t present  					:1;
	uint32_t read_write  				:1;
	uint32_t user_supervisor  			:1;
	uint32_t write_through  			:1;
	uint32_t cache_disabled				:1;
	uint32_t accessed					:1;
	uint32_t reserved  					:1;
	uint32_t page_size  				:1;
	uint32_t global_page				:1;
	uint32_t avail						:3;
	uint32_t page_table_base_address	:20;
}pde_t;

/* struct for page table entry*/
typedef struct __attribute__((packed)) pte{
	uint32_t present  					:1;
	uint32_t read_write  				:1;
	uint32_t user_supervisor  			:1;
	uint32_t write_through  			:1;
	uint32_t cache_disabled				:1;
	uint32_t accessed					:1;
	uint32_t dirty  					:1;
	uint32_t pat		  				:1;
	uint32_t global_page				:1;
	uint32_t avail						:3;
	uint32_t page_table_base_address	:20;
}pte_t;

#include "syscall.h"

//Declearing the space for page dir and page table
extern pde_t page_dir[PROGRAM_COUNT][PGGE_KB_SIZE];
extern pte_t page_tbl1[PROGRAM_COUNT][PGGE_KB_SIZE];
extern pte_t page_tbl2[PROGRAM_COUNT][PGGE_KB_SIZE];

void page_init(void);

#endif /* _PAGE_H */

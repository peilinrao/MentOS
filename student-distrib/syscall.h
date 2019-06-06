#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types.h"

#define PROGRAM_IMG 0x08048000
#define PROGRAM_START 0x08000000
#define PROGRAM_END 0x08400000
#define PROGRAM_SIZE 0x400000
#define PROGRAM_COUNT 8
#define MAX_PGM_SIZE 0x3B8000
#define KN_PG_SIZE 8192
#define MAX_PID 7
#define ARG_LEN 1024
#define PCB_FDR 0x7FE000
#define PCB_FDR2 0x7F0000
#define PCB_TO_STACK 0x1ffc
#define MAX_FILR_OBJ 8
#define CONTEXT_IRET_TOP 0x1FEC
#define CONTEXT_POPA_TOP 0x1FCC
#define DEFAULT_ESP 0x83ffffc
#define CONTEXT_SIZE 52
#define CHAR_IN_FILE_NAME 32

#include "page.h"

typedef struct file_op_tbl_ptr {
	int32_t (*open)(void);
	int32_t (*read)(uint32_t fd, void* buf, int32_t nbytes);
	int32_t (*write)(uint32_t fd, const void* buf, int32_t nbytes);
	int32_t (*close)(void);
} file_op_tbl_ptr_t;

typedef struct file_descriptor {
	file_op_tbl_ptr_t* file_op_tbl_ptr;
	uint32_t inode;
	uint32_t file_pos;
	uint32_t flags;
} fd_t;

typedef struct pcb{
	uint32_t valid;
	int8_t par_pid;
	int8_t this_pid;
	fd_t fd_arr[MAX_FILR_OBJ];
	uint8_t name[CHAR_IN_FILE_NAME];
	uint8_t arg[ARG_LEN];
	uint32_t saved_esp;
	uint32_t saved_ebp;
	uint8_t vidmapped;
	uint8_t scheduled;
	uint32_t retval;
	uint32_t terminal_num;
} pcb_t;

typedef struct pcb_addr_finder{
	pcb_t pcb;
	uint8_t empty[KN_PG_SIZE-sizeof(pcb_t)];
} pcb_addr_finder_t;

typedef struct stack_context{
	uint8_t empty[KN_PG_SIZE - CONTEXT_SIZE];
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
}stack_context_t;

int32_t syscall(int32_t argA, void* argB, void* argC, void* argD);
int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command, int32_t flag_o_ret);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open(const int8_t* filename);
int32_t close(int32_t fd);
int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t sigum, void* handler_address);
int32_t sigreturn(void);

extern pcb_t* current_pcb;

#endif /* _SYSCALL_H */

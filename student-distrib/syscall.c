#include "types.h"
#include "kb.h"
#include "syscall.h"
#include "fs.h"
#include "rtc.h"
#include "page.h"
#include "x86_desc.h"
#include "terminal.h"

#define FAIL -1
#define SUCC 0

pcb_t* current_pcb = (pcb_t*)PCB_FDR2;

/* syscall:
		DESCRIPTION: decide what to do base on the value in eax
		INPUT: eax, ebx, ecx, edx
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: call function based on eax
*/

int32_t syscall(int32_t argA, void* argB, void* argC, void* argD){
	switch (argA){
		case 1: return halt((uint8_t)((uint32_t)argB));
		case 2: return execute((const uint8_t*)argB, -1);
		case 3: return read((int32_t)argB, argC, (int32_t)argD);
		case 4: return write((int32_t)argB, argC, (int32_t)argD);
		case 5: return open((const int8_t*)argB);
		case 6: return close((int32_t)argB);
		case 7: return getargs((uint8_t*)argB, (int32_t)argC);
		case 8: return vidmap((uint8_t**)argB);
		case 9: return set_handler((int32_t)argB, argC);
		case 10: return sigreturn();
		default: return FAIL;
	}
}


/* halt:
		DESCRIPTION: halt task
		INPUT: status: the status of the task in fd
		OUTPUT: NONE
		RETURN VALUE: FAIL or SUCC
		SIDE EFFECTS: Cannot halt shell if it is the last shell
*/

int32_t halt(uint8_t status){
	int i;
	//                  ↓ a mask
	int32_t result = 0xFF & status;
	
	for(i = 0; i < MAX_FILR_OBJ; i++){
		if(current_pcb->fd_arr[i].file_op_tbl_ptr){
			(*((current_pcb->fd_arr[i].file_op_tbl_ptr)->close))();
		}
	}
	if(current_pcb->vidmapped){
		page_dir[(uint32_t)(current_pcb->this_pid)][(PROGRAM_START >> PAGEDIR_OFFSET) + 1].user_supervisor = 0;
		page_tbl2[(uint32_t)(current_pcb->this_pid)][0].user_supervisor = 0;
	}
	current_pcb->valid = 0;
	current_pcb->scheduled = 0;
	if(current_pcb->par_pid != -1){
		pcb_addr_finder_t* pcb_ptr = (pcb_addr_finder_t*)(PCB_FDR);
		pcb_ptr -= current_pcb->par_pid;
		pcb_ptr->pcb.scheduled = 1;
		pcb_ptr->pcb.retval = result;
	} else {
		asm volatile ("movl $0x7efffc, %esp");
		execute((uint8_t*)"shell", current_pcb->terminal_num);
	}
	asm volatile ("jmp new_process2");
	return result;
}

/* execute:
		DESCRIPTION: execute task based on a string
		INPUT: command: the string of the name of the task
		OUTPUT: NONE
		RETURN VALUE: FAIL or SUCC
		SIDE EFFECTS: NONE
*/
int32_t execute(const uint8_t* command, int32_t flag_o_ret){
	int8_t pid = 0;
	uint32_t count = 0;
	//           ↓ mentioned in doc
	uint8_t temp[40];
	uint32_t EIP;
	dentry_t food;
	pcb_t* new_pcb;
	stack_context_t* new_context;
	
	pcb_addr_finder_t* pcb_ptr = (pcb_addr_finder_t*)(PCB_FDR); //8M-8K
	while(1){
		if(pid>MAX_PID) return FAIL;
		//1 means it is not available
		//0 means it is available
		if(pcb_ptr->pcb.valid == 0){
			break;
		}else{
			pcb_ptr--;
			pid++;
		}
	}
	memset(pcb_ptr, 0, KN_PG_SIZE);
	
	while(*command == ' ') command++;
	while(*command && *command != ' '){
		pcb_ptr->pcb.name[count] = *command;
		command++;
		count++;
		if(count == CHAR_IN_FILE_NAME) return FAIL;
	}
	if(*command == ' '){
		while(*command == ' ') command++;
		count = 0;
		while(*command){
			pcb_ptr->pcb.arg[count] = *command;
			command++;
			count++;
			if(count == ARG_LEN) return FAIL;
		}
		count--;
		while(pcb_ptr->pcb.arg[count] == ' '){
			pcb_ptr->pcb.arg[count] = '\0';
			count--;
		}
	}
	
	if(read_dentry_by_name((int8_t*)(pcb_ptr->pcb.name), &food) == FAIL) return FAIL;
	//40 is bcause we only need the information in first 40 bytes, specified in doc
	read_data(food.inode, 0, temp, 40);
	//              ↓ 'DEL'
	if(temp[0] != 0x7f || temp[1] != 'E' || temp[2] != 'L' || temp[3] != 'F') return FAIL;
	//                        ↓ specified in doc
	EIP = *((uint32_t*)&(temp[24]));
	
	new_pcb = &(pcb_ptr->pcb);
	new_context = (stack_context_t*)(pcb_ptr);
	new_pcb->valid = 1;
	new_pcb->scheduled = 1;
	new_pcb->fd_arr[0].file_op_tbl_ptr = &kb_op_tbl;
	new_pcb->fd_arr[1].file_op_tbl_ptr = &kb_op_tbl;
	//                ↓ fail-safe default retval
	new_pcb->retval = 256;
	new_pcb->this_pid = pid;
	if(flag_o_ret < 0){
		current_pcb->scheduled = 0;
		new_pcb->par_pid = current_pcb->this_pid;
		new_pcb->terminal_num = current_pcb->terminal_num;
	} else {
		new_pcb->par_pid = -1;
		new_pcb->terminal_num = flag_o_ret;
	}
	
	switch_term(current_terminal);
	
	asm volatile ("movl %0, %%cr3"
					:
					: "a"(page_dir[(uint32_t)(pid)])
					: "memory", "cc"
	);
	read_data(food.inode, 0, (void*)(PROGRAM_IMG), MAX_PGM_SIZE);
	asm volatile ("movl %0, %%cr3"
					:
					: "a"(page_dir[(uint32_t)(current_pcb->this_pid)])
					: "memory", "cc"
	);
	
	new_pcb->saved_ebp = (uint32_t)new_context + CONTEXT_IRET_TOP;
	new_pcb->saved_esp = (uint32_t)new_context + CONTEXT_POPA_TOP;
	
	new_context->edi = 0;
	new_context->esi = 0;
	new_context->ebp = (uint32_t)new_context + CONTEXT_IRET_TOP;
	new_context->esp = (uint32_t)new_context + CONTEXT_IRET_TOP;
	new_context->ebx = 0;
	new_context->edx = 0;
	new_context->ecx = 0;
	new_context->eax = 0;
	new_context->retaddr = EIP;
	new_context->cs = USER_CS;
	//                     ↓ default flag, only IF
	new_context->flags = 0x202;
	new_context->esp_old = DEFAULT_ESP;
	new_context->ss = USER_DS;
	
	if(flag_o_ret < 0){
		asm volatile ("int $0x20");
		return current_pcb->retval;
	} else {
		return SUCC;
	}
}


/* read:
		DESCRIPTION: read
		INPUT: fd: the fd number of the task
		       buf: the buffer used in the read function of a task
		       nbytes: also the needed arg for read function of a task
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: Call read function of a certain task
*/
int32_t read(int32_t fd, void* buf, int32_t nbytes){
	//sti();
	if(fd < 0 || fd >= MAX_FILR_OBJ || fd == 1) return FAIL;
	if(current_pcb->fd_arr[fd].file_op_tbl_ptr)
	return (*((current_pcb->fd_arr[fd].file_op_tbl_ptr)->read))(fd, buf, nbytes);
	return FAIL;
}

/* write:
		DESCRIPTION: a general write function
		INPUT: fd: the fd number of the task
		       buf: the buffer used in the write function of a task
		       nbytes: also the needed arg for write function of a task
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: Call write function of a certain task
*/
int32_t write(int32_t fd, const void* buf, int32_t nbytes){
	if(fd < 0 || fd >= MAX_FILR_OBJ || fd == 0) return FAIL;
	if(current_pcb->fd_arr[fd].file_op_tbl_ptr)
	return (*((current_pcb->fd_arr[fd].file_op_tbl_ptr)->write))(fd, buf, nbytes);
	return FAIL;
}

/* open:
		DESCRIPTION: open a task
		INPUT: name: a string of name of the task we want to open
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: assign pcb for the task
*/
int32_t open(const int8_t* name){
	int32_t i;
	for(i = 0; i < MAX_FILR_OBJ; i++){
		if(!current_pcb->fd_arr[i].file_op_tbl_ptr) break;
	}
	if (i == MAX_FILR_OBJ) return FAIL;
	memset(&(current_pcb->fd_arr[i]), 0, sizeof(fd_t));
	if(name[0] == 'r' && name[1] == 't' && name[2] == 'c' && name[3] == '\0'){
		current_pcb->fd_arr[i].file_op_tbl_ptr = &rtc_op_tbl;
	}else if(name[0] == 'k' && name[1] == 'b' && name[2] == '\0'){
		current_pcb->fd_arr[i].file_op_tbl_ptr = &kb_op_tbl;
	}else{
		//It is a file!
		dentry_t food;
		if (read_dentry_by_name (name, &food) == FAIL) return FAIL;
		if(food.type == DIR) {
			current_pcb->fd_arr[i].file_op_tbl_ptr = &dir_op_tbl;
		} else if(food.type == FILE){
			current_pcb->fd_arr[i].file_op_tbl_ptr = &file_op_tbl;
		} else {
			return FAIL;
		}
		current_pcb->fd_arr[i].inode = food.inode;
	}
	if(FAIL == (*((current_pcb->fd_arr[i].file_op_tbl_ptr)->open))()) return FAIL;
	return i;
}

/* close:
		DESCRIPTION: close a task
		INPUT: name: a string of name of the task we want to close
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: clear the pcb for the task
*/
int32_t close(int32_t fd){
	if(fd < 2 || fd >= MAX_FILR_OBJ) return FAIL;
	if(current_pcb->fd_arr[fd].file_op_tbl_ptr){
		if((*((current_pcb->fd_arr[fd].file_op_tbl_ptr)->close))() == FAIL) return FAIL;
		memset(&(current_pcb->fd_arr[fd]), 0, sizeof(fd_t));
		return SUCC;
	}
	return FAIL;
}


/* getargs:
		DESCRIPTION: get arguments of a task
		INPUT: buf: the give in buffer
		       nbytes: number of bytes the buf can hold
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: NONE
*/
int32_t getargs(uint8_t* buf, int32_t nbytes){
	//printf("%s", current_pcb->arg);
	if(!buf) return -1;
	if(!current_pcb->arg[0]) return FAIL;
	int i = 0;
	while(nbytes > 0 && i < ARG_LEN && current_pcb->arg[i]){
		*buf = current_pcb->arg[i];
		buf++;
		i++;
		nbytes--;
	}
	if(current_pcb->arg[i]) return FAIL;
	*buf = '\0';
	return SUCC;
}

/* vidmap:
		DESCRIPTION: set video memeory to user accessible memory space
		INPUT: screen_start: modified by the function to save the pointer to the
												 start of new video memory address
		OUTPUT: NONE
		RETURN VALUE: SUCC: upon success
									FAIL: upon fail
		SIDE EFFECTS: sets the address of new virtual video memory in screen_start
*/
int32_t vidmap(uint8_t** screen_start){
	if((uint32_t)screen_start < PROGRAM_START || (uint32_t)screen_start > PROGRAM_END) return FAIL;
	clear();
	current_pcb->vidmapped = 1;
	page_dir[(uint32_t)(current_pcb->this_pid)][(PROGRAM_START >> PAGEDIR_OFFSET) + 1].user_supervisor = 1;
	page_tbl2[(uint32_t)(current_pcb->this_pid)][0].user_supervisor = 1;
	// set up control registers to initialize paging
	*screen_start = (uint8_t*)(VIDEOV);
	asm volatile ("movl %0, %%cr3"
					:
					: "a"(page_dir[(uint32_t)(current_pcb->this_pid)])
					: "memory", "cc"
	);
	return VIDEOV;
}

int32_t set_handler(int32_t sigum, void* handler_address){
	//asm volatile ("sti");
	return SUCC;
}

int32_t sigreturn(void){
	//asm volatile ("sti");
	return SUCC;
}

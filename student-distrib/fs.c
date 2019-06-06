#include "fs.h"
#include "lib.h"
#include "syscall.h"

#define SUCC 0
#define FAIL -1

file_op_tbl_ptr_t file_op_tbl = {file_open, file_read, file_write, file_close};
file_op_tbl_ptr_t dir_op_tbl = {file_open, dir_read, dir_write, file_close};

/** variable that keep track of the location and param of fs */
uint32_t fs_boot_blk_addr = 0;
uint32_t dir_entries_num = 0;
uint32_t inode_num = 0;
uint32_t data_blk_num = 0;
uint32_t data_blk_addr = 0;

/** init the fs and its param
@addr: addr of the boot block
*/
void fs_init(uint32_t addr){
	fs_boot_blk_addr = addr;
	boot_blk_t* temp = (boot_blk_t*)fs_boot_blk_addr;
	dir_entries_num = temp->dir;
	inode_num = temp->inode;
	data_blk_num = temp->data;
	data_blk_addr = fs_boot_blk_addr + BLK_SIZE * (inode_num + 1);
	printf("FS inited at %x!\n", fs_boot_blk_addr);
}

/**
@fname: name to be found, 0term or 32char fixed length
@n_dentry: struct to be filled
@return: FAIL or SUCC
*/
int32_t read_dentry_by_name (const int8_t* fname, dentry_t* n_dentry){
	if(!fs_boot_blk_addr) return FAIL;
	dentry_t* current_dentry = (dentry_t*)(fs_boot_blk_addr + DENTRY_START);
	int i, j;
	for(i = 0; i < dir_entries_num; i++){
		for(j = 0; j < CHAR_IN_FILE_NAME; j++){
			if(!fname[j] || fname[j] == '\n' || !(current_dentry->name[j]) || j == CHAR_IN_FILE_NAME-1){
				if(fname[j] == current_dentry->name[j]){
					for(i = 0; i < CHAR_IN_FILE_NAME; i++){
						n_dentry->name[i] = current_dentry->name[i];
					}
					n_dentry->type = current_dentry->type;
					n_dentry->inode = current_dentry->inode;
					return SUCC;
				} else break;
			}
			if(fname[j] == current_dentry->name[j]) continue;
			else break;
		}
		current_dentry++;
	}
	return FAIL;
}

/**
@index: index into the file table
@n_dentry: struct to be filled
@return: FAIL or SUCC
*/
int32_t read_dentry_by_index (uint32_t index, dentry_t* n_dentry){
	if(!fs_boot_blk_addr || index >= dir_entries_num) return FAIL;
	dentry_t* current_dentry = (dentry_t*)(fs_boot_blk_addr + DENTRY_START * (1 + index));
	int i;
	for(i = 0; i < CHAR_IN_FILE_NAME; i++){
		n_dentry->name[i] = current_dentry->name[i];
	}
	n_dentry->type = current_dentry->type;
	n_dentry->inode = current_dentry->inode;
	return SUCC;
}

/** read continious data from fs
@inode: the inode number for the file
@offset: offset from the start of file
@buf: buf to be filled
@length: length to filled
@return: number of bytes read
*/
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
	if(!fs_boot_blk_addr) return FAIL;
	int32_t retval = 0;
	inode_t* the_inode = (inode_t*)(fs_boot_blk_addr + BLK_SIZE * (inode + 1));
	if(length + offset > the_inode->length) length = the_inode->length - offset;
	uint32_t data_blk_idx = the_inode->data_block_index[offset / BLK_SIZE];
	uint8_t* data_addr = (uint8_t*)(data_blk_addr + data_blk_idx * BLK_SIZE + offset % BLK_SIZE);
	while(length != 0){
		if(offset % BLK_SIZE == 0){
			data_blk_idx = the_inode->data_block_index[offset / BLK_SIZE];
			data_addr = (uint8_t*)(data_blk_addr + BLK_SIZE * data_blk_idx);	//switch data block
		}
		*buf = *data_addr;
		buf++;
		retval++;
		data_addr++;
		length--;
		offset++;
	}
	//printf("retval: %d", retval);
	return retval;
}

/**
@fd file handler
@buf: buf to be filled
@length: length to filled
*/
int32_t file_read(uint32_t fd, void* buf, int32_t nbytes){
	int32_t retval;
	retval = read_data(current_pcb->fd_arr[fd].inode, current_pcb->fd_arr[fd].file_pos, (uint8_t*)buf, nbytes);
	current_pcb->fd_arr[fd].file_pos += retval;
	return retval;
}

/** auto fail */
int32_t file_write(uint32_t fd, const void* buf, int32_t count){
	(void)fd;
	(void)buf;
	(void)count;
	return FAIL;
}

/** nothing */
int32_t file_open(){
	return SUCC;
}

/** nothing */
int32_t file_close(){
	return SUCC;
}

/**
@fd file handler
@buf: buf to be filled
@length: length to filled
*/
int32_t dir_read(uint32_t fd, void* buf, int32_t nbytes){
	dentry_t dentry;
	if(read_dentry_by_index(1 + current_pcb->fd_arr[fd].file_pos, &dentry) == FAIL) return 0;
	current_pcb->fd_arr[fd].file_pos += 1;
	int i = 0;
	while(nbytes > 0 && i < CHAR_IN_FILE_NAME){
		((uint8_t*)buf)[i] = dentry.name[i];
		if(!dentry.name[i]) break;
		i++;
		nbytes--;
	}
	return i;
}

/** auto fail */
int32_t dir_write(uint32_t fd, const void* buf, int32_t count){
	(void)fd;
	(void)buf;
	(void)count;
	return FAIL;
}

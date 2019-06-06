#ifndef _FS_H
#define _FS_H

#include "types.h"
#include "syscall.h"

#define DIR 1
#define FILE 2

#define CHAR_IN_FILE_NAME 32
#define BLK_SIZE 4096
#define DENTRY_START 64

/** struct for all data strtcture */
typedef struct boot_blk{
	uint32_t dir;
	uint32_t inode;
	uint32_t data;
	uint32_t reserved[13];
} boot_blk_t;

typedef struct dentry {
	uint8_t name[32];
	uint32_t type;
	uint32_t inode;
	uint32_t reserved[6];
} dentry_t;

typedef struct inode {
	uint32_t length;
	uint32_t data_block_index[1023];
} inode_t;

int32_t read_dentry_by_name (const int8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
void fs_init(uint32_t addr);

int32_t file_read(uint32_t fd, void* buf, int32_t nbytes);
int32_t file_write(uint32_t fd, const void* buf, int32_t count);
int32_t dir_read(uint32_t fd, void* buf, int32_t nbytes);
int32_t dir_write(uint32_t fd, const void* buf, int32_t count);
int32_t file_open(void);
int32_t file_close(void);

extern file_op_tbl_ptr_t file_op_tbl;
extern file_op_tbl_ptr_t dir_op_tbl;

#endif /* _FS_H */

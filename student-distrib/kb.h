#ifndef _KB_H
#define _KB_H

#include "types.h"
#include "lib.h"
#include "syscall.h"

#define KB_PORT           0x60
#define KB_IRQ_NUM        0x21
#define BUFFER_SIZE           128
#define BUFFER_SIZE_LESS      127
#define SCAN_CODE_2           86
#define PRINTABLE_KK          0x3A

#define CTL                  0x1D
#define LEFT_CMD             0x5B
#define RIGHT_CMD            0x5C
#define LEFT_SHIFT           0x2A
#define RIGHT_SHIFT          0x36
#define ALT                  0x38
#define CAPSLOCK             0x3A
#define BACKSPACE            0x0E
#define ENTER                0x0D

void kb_enable_irq(void);

void kb_disable_irq(void);

void kb_irq();

int32_t kb_read(uint32_t fd, void* buf, int32_t nbytes);

int32_t kb_write(uint32_t fd, const void* buf, int32_t nbytes);

int32_t kb_open (void);

int32_t kb_close (void);

void kb_init(void);

extern file_op_tbl_ptr_t kb_op_tbl;

void kb_flush(void);

#endif /* _KB_H */

#ifndef _RTC_H
#define _RTC_H

#include "types.h"
#include "lib.h"
#include "syscall.h"

#define RTC_PORT_MAR      0x70
#define RTC_PORT_MDR      0x71
#define RTC_IRQ_NUM       0x28

extern volatile uint8_t read_flag;

int32_t rtc_read(uint32_t fd, void* buf, int32_t nbytes);
int32_t rtc_write(uint32_t fd, const void* buf, int32_t count);
int32_t rtc_open(void);
int32_t rtc_close(void);

void rtc_init(void);

void rtc_irq(void);
/* enable rtc interrupts */
void rtc_enable_irq(void);
/* disable rtc interrupts */
void rtc_disable_irq(void);

void rtc_flush(void);

extern file_op_tbl_ptr_t rtc_op_tbl;

#endif /* _RTC_H */

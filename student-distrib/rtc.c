#include "types.h"
#include "lib.h"
#include "rtc.h"
#include "i8259.h"
#include "syscall.h"
#include "pit.h"

file_op_tbl_ptr_t rtc_op_tbl = {rtc_open, rtc_read, rtc_write, rtc_close};

char enable[PROGRAM_COUNT] = {0};
int32_t reload[PROGRAM_COUNT] = {0};
int32_t count[PROGRAM_COUNT] = {0};

//volatile uint8_t read_flag = 0;

/* rtc_read:
		DESCRIPTION: it initialize the rtc
		INPUT: fd: dummy
		 			 buf: dummy
					 nbytes: dummy
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: Stuck the user here.
*/
int32_t rtc_read(uint32_t fd, void* buf, int32_t nbytes){
	(void)fd;
	(void)buf;
	(void)nbytes;
	if(!enable[(uint32_t)(current_pcb->this_pid)]) return 0;
	if(count[(uint32_t)(current_pcb->this_pid)] < reload[(uint32_t)(current_pcb->this_pid)]){
		current_pcb->scheduled = 0;
		//tprintf("hmmm %x descheduled\n", current_pcb);
		asm volatile ("int $0x20");
	}
	return 0;
}

/* rtc_write:
		DESCRIPTION: it initialize the rtc
		INPUT: fd: dummy
		 			 buf: dummy
					 count: the rate of rtc interrupt, from 0x01 to 0x0F
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: it set the rate of rtc interrupt according to count
*/
int32_t rtc_write(uint32_t fd, const void* buf, int32_t nbytes){
	(void)fd;
	(void)buf;
	
	if(nbytes < 1) return -1;
	/*
	//                                  ↓this is a mask
	int32_t freq = (*((int32_t*)buf)) & 0x7fff;
	
	int32_t cnt = 0;
	while(freq){
		freq >>= 1;
		cnt++;
	}
	//            ↓time scale factor
	int32_t cmp = 18 - cnt;
	//Count should be larger than 2 but not larger than 15
	if(cmp < 0x01 || cmp > 0x0F) return -1;
	cli();
	outb(0x8A, RTC_PORT_MAR);
	char prev = inb(RTC_PORT_MDR);
	outb(0x8A, RTC_PORT_MAR);
	//0xF0 is a mask, the following line set the frequency
	outb((prev & 0xF0) | cmp, RTC_PORT_MDR);
	sti();
	return 0;
	*/
	//                                    ↓ a mask
	int32_t freq = (*((int32_t*)buf)) & 0x1fff;
	//                                           ↓ rtc base freq
	reload[(uint32_t)(current_pcb->this_pid)] = (1024 / freq) - 1;
	count[(uint32_t)(current_pcb->this_pid)] = 0;
	enable[(uint32_t)(current_pcb->this_pid)] = 1;
	return 0;
}


/* rtc_open:
		DESCRIPTION: it initialize the rtc
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: it set the rate of rtc interrupt to 2Hz
*/
int32_t rtc_open(){
	rtc_enable_irq();
	//read_flag = 0;
	return 0;
}

/* rtc_close:
		DESCRIPTION: disable rtc interrupts
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: disable rtc interrupts
*/
int32_t rtc_close(){
	int i;
	char flag = 0;
	for(i = 0; i < PROGRAM_COUNT; i++){
		flag |= enable[i];
	}
	if(!flag) rtc_disable_irq();
	enable[(uint32_t)(current_pcb->this_pid)] = 0;
	return 0;
}


/* rtc_irq:
		DESCRIPTION: rtc interrupt handler
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: call test_interrupts upon interrupts from rtc
*/
void rtc_irq(){
  outb(0x0C, RTC_PORT_MAR);
  inb(RTC_PORT_MDR);
  //tprintf("rtc interrupt received! ");
	//read_flag = 0;
	int i;
	char flag = 0;
	pcb_addr_finder_t* pcb_ptr = (pcb_addr_finder_t*)(PCB_FDR); //8M-8K
	for(i = 0; i < PROGRAM_COUNT; i++, pcb_ptr--){
		//tprintf("a");
		if(enable[i]){
			//tprintf("X");
			count[i]++;
			if(count[i] >= reload[i]){
				//tprintf("hmmm %x ", pcb_ptr);
				pcb_ptr->pcb.scheduled = 1;
				//tprintf("rescheduled!\n");
				count[i] = 0;
				flag = 1;
			}
		}
	}
	if(sys_halted && flag){
		rtc_flush();
		asm volatile ("jmp new_process2");
	}
}

/* rtc_flush:
		DESCRIPTION: send_eoi to the irq
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: NONE
*/
void rtc_flush(){
	outb(0x0C, RTC_PORT_MAR);
	inb(RTC_PORT_MDR);
	send_eoi(2);
	send_eoi(RTC_IRQ_NUM);
}


/* rtc_enable_irq:
		DESCRIPTION: enable rtc interrupts
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: enable rtc interrupts
*/

void rtc_enable_irq(){
  outb(0x8B, RTC_PORT_MAR);
  uint8_t temp = inb(RTC_PORT_MDR);
  temp |= 0x40;
  outb(0x8B, RTC_PORT_MAR);
  outb(temp, RTC_PORT_MDR);
  enable_irq(8);
  outb(0x0C, RTC_PORT_MAR);
  inb(RTC_PORT_MDR);
}


/* rtc_disable_irq:
		DESCRIPTION: disable rtc interrupts
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: disable rtc interrupts
*/

void rtc_disable_irq(){
  outb(0x8B, RTC_PORT_MAR);
  uint8_t temp = inb(RTC_PORT_MDR);
  temp &= ~0x40;
  outb(0x8B, RTC_PORT_MAR);
  outb(temp, RTC_PORT_MDR);
  disable_irq(8);
}

/* rtc_init:
		DESCRIPTION: initialize rtc interrupt
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: NONE
*/
void rtc_init(){
	int8_t selected_rate = 0x03; //initialize to 3, indicating 8kHz
	outb(0x8A,RTC_PORT_MAR);
	char prev = inb(RTC_PORT_MDR);
	outb(0x8A, RTC_PORT_MAR);
	//0xF0 is a mask, the following line set the frequency
	outb((prev & 0xF0) | selected_rate,RTC_PORT_MDR);
	//rtc_enable_irq();
}

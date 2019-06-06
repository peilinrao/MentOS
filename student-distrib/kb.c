#include "kb.h"
#include "types.h"
#include "lib.h"
#include "i8259.h"
#include "syscall.h"
#include "terminal.h"
#include "pit.h"

file_op_tbl_ptr_t kb_op_tbl = {kb_open, kb_read, kb_write, kb_close};

static uint8_t charBuffer[MAX_TERM][BUFFER_SIZE];
static uint8_t ctlFLAG = 0;
static uint8_t shiftFLAG = 0;
static uint8_t altFLAG = 0;
static uint8_t cmdFLAG = 0;
static uint8_t CapsLockFLAG = 0;
static uint8_t charCount[MAX_TERM] = {0};
int32_t freez[PROGRAM_COUNT] = {-1};

/* this array is used to convert scancode 2 data into the corresponding ASCII values */
//The total number of keys in scancode2 is 86
const uint8_t sc2ToASCII[SCAN_CODE_2] = {0x00, 0x1B, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x2D, 0x3D, 0x08,
                                0x09, 0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69, 0x6f, 0x70, 0x5b, 0x5d, '\n', 0x00,
                                0x61, 0x73, 0x64, 0x66, 0x67, 0x68, 0x6a, 0x6b, 0x6c, 0x3b, 0x27, 0x60, 0x00, 0x5c, 0x7a,
                                0x78, 0x63, 0x76, 0x62, 0x6e, 0x6d, 0x2c, 0x2e, 0x2f, 0x00, 0x2a, 0x00, 0x20, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x38, 0x39, 0x2d,
                                0x34, 0x35, 0x36, 0x2b, 0x31, 0x32, 0x33, 0x30, 0x2e, 0x00, 0x00};

//128 is because there is 128 char in ascii table
const uint8_t ASCII_shift[128] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                                  0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
                                  0x1E, 0x1F,
                                  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x22, 0x28, 0x29, 0x2A, 0x2B, 0x3C, 0x5F, 0x3E,
                                  0x3F, 0x29, 0x21, 0x40, 0x23, 0x24, 0x25, 0x5E, 0x26, 0x2A, 0x28, 0x3A, 0x3A, 0x3C, 0x2B,
                                  0x3E, 0x3F,
                                  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
                                  0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x7B, 0x7C, 0x7D,
                                  0x5E, 0x5F,
                                  0x7E, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
                                  0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x7B, 0x7C, 0x7D,
                                  0x7E, 0x7F};

/* kb_enable_irq:
		DESCRIPTION: enable keyboard interrupts
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: enable keyboard interrupts
*/
void kb_enable_irq(){
  enable_irq(1);
  inb(KB_PORT);
}


/* kb_flush:
		DESCRIPTION: send eoi to the kb irq
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: NONE
*/
void kb_flush(){
	inb(KB_PORT);
	send_eoi(KB_IRQ_NUM);
}

/* kb_disable_irq:
		DESCRIPTION: disable keyboard interrupts
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: disable keyboard interrupts
*/
void kb_disable_irq(){
  disable_irq(1);
}


/* kb_irq:
		DESCRIPTION: keyboard interrupt handler
		INPUT: NONE
		OUTPUT: print to screen
		RETURN VALUE: NONE
		SIDE EFFECTS: deal with the pressed key
*/
void kb_irq(){
    cli();
  	uint32_t read_value = inb(KB_PORT);
	int i;
    //This is a mask to check if the first bit is 1
    if(!(read_value & 0x80)){
      //0x1c is enter
      if(read_value == 0x1c){
        
        charBuffer[current_terminal][charCount[current_terminal]] = '\n';
		charCount[current_terminal]++;
		tputc('\n');
		pcb_addr_finder_t* pcb_ptr = (pcb_addr_finder_t*)(PCB_FDR);
		for(i = 0; i < PROGRAM_COUNT; i++, pcb_ptr--){
			if(freez[i] == current_terminal){
				freez[i] = -1;
				pcb_ptr->pcb.scheduled = 1;
				if(sys_halted){
					kb_flush();
					asm volatile ("jmp new_process2");
				}
			}
		}
        return;
      }
      //If control+L is pressed, clear the screen
      //0x26 is L
      if(ctlFLAG && read_value == 0x26){
        clear();
		//tprintf("lol: %d", charCount[current_terminal]);
		for(i = 0; i < charCount[current_terminal]; i++){
			tputc(charBuffer[current_terminal][i]);
		}
        return;
      }
	  
	  //Alt+F1:0x3B/F2:0x3C/F3:0x3D
	  //Terminal switch
	  if(read_value == 0x3B && altFLAG){
		  switch_term(0);
		  return;
	  }
	  if(read_value == 0x3C && altFLAG){
		  switch_term(1);
		  return;
	  }
	  if(read_value == 0x3D && altFLAG){
		  switch_term(2);
		  return;
	  }

  		switch (read_value){
        case CTL:         {ctlFLAG++;      break;}
        case LEFT_CMD:
        case RIGHT_CMD:   {cmdFLAG++;      break;}
        case LEFT_SHIFT:
        case RIGHT_SHIFT: {shiftFLAG++;    break;}
        case ALT:         {altFLAG++;      break;}
        case CAPSLOCK:    {CapsLockFLAG++; break;}
        case BACKSPACE:   {
			if(charCount[current_terminal] != 0){
				charCount[current_terminal]--;
				clear_one_char();
			}
			break;
		}
        default:{
          if (charCount[current_terminal]>=BUFFER_SIZE_LESS || read_value >= PRINTABLE_KK) break;
          uint8_t temp = sc2ToASCII[read_value];
          //0x61(A) and 0x7A(Z) is for checking if the input is a lower-case letter
          if ((shiftFLAG) || (CapsLockFLAG % 2 && (temp >= 0x61) && (temp <= 0x7A)))
            temp = ASCII_shift[temp];
          charBuffer[current_terminal][charCount[current_terminal]] = temp;
          tputc(temp);
          charCount[current_terminal]++;
          break;
        }
      }
  	}
    else {
		//MSB for release
      //Now we are releasing, get rid of the first bit using -= 0x80
      read_value -= 0x80;
      switch (read_value){
        case CTL:         {ctlFLAG--;   break;}
        case LEFT_CMD:
        case RIGHT_CMD:   {cmdFLAG--;   break;}
        case LEFT_SHIFT:
        case RIGHT_SHIFT: {shiftFLAG--; break;}
        case ALT:         {altFLAG--;   break;}
        default: ; //nothing
      }
    }
}


/* kb_open:
		DESCRIPTION: Enable keyboard
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: it calls kb_enable_irq
*/
int32_t kb_open(){
    cli();
    //kb_enable_irq();
    memset(charBuffer[current_pcb->terminal_num], 0, BUFFER_SIZE); // initialize the charBuffer[current_pcb->terminal_num] to all 0s

	freez[(uint32_t)(current_pcb->this_pid)] = -1;
    charCount[current_pcb->terminal_num] = 0;
    sti();
    return 0;
}

/* kb_read:
		DESCRIPTION: It passes the buffer to user
		INPUT: fd: dummy
           buf: the user level buffer
           nbytes: desired lenggt
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS:
*/
int32_t kb_read(uint32_t fd, void* buf, int32_t nbytes){
    (void)fd;
    int32_t i;
	freez[(uint32_t)(current_pcb->this_pid)] = current_pcb->terminal_num;
	if(freez[(uint32_t)(current_pcb->this_pid)] >= 0){
		current_pcb->scheduled = 0;
		asm volatile ("int $0x20");
	}
	if (nbytes>charCount[current_pcb->terminal_num]){
		//Pass string with size according to charCount[current_pcb->terminal_num]
		for (i = 0; i < charCount[current_pcb->terminal_num]; i++){
			((uint8_t*)buf)[i] = charBuffer[current_pcb->terminal_num][i];
		}
	}else{
		//Pass string with size according to nbytes
		for (i = 0; i < nbytes; i++){
			((uint8_t*)buf)[i] = charBuffer[current_pcb->terminal_num][i];
		}
	}
	memset(charBuffer[current_pcb->terminal_num], 0, BUFFER_SIZE);
	charCount[current_pcb->terminal_num] = 0;
    return i;
}

/* kb_write:
		DESCRIPTION: It prints out the string to the screen
		INPUT: fd: dummy
           buf: the string to be printed
           nbytes: the length of desired out put
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS:
*/
int32_t kb_write(uint32_t fd, const void* buf, int32_t nbytes){
    (void)fd;
    int32_t i;
    if (nbytes < 0){
      return -1;
    }
    for (i = 0; i < nbytes; i++){
      if (!((uint8_t*)buf)[i]) break;
      putc(((uint8_t*)buf)[i]);
    }
    return 0;
}

/* kb_close:
		DESCRIPTION: It disable the keyboard interrupt
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: It calls kb_disable_irq();
*/
int32_t kb_close(){
    return 0;
}


/* kb_init:
		DESCRIPTION: It initializes the keyboard
		INPUT: NONE
		OUTPUT: NONE
		RETURN VALUE: NONE
		SIDE EFFECTS: It calls kb_enable_irq();
*/
void kb_init(void){
	int i;
	for(i = 0; i < MAX_TERM; i++){
		memset(charBuffer[i], 0, BUFFER_SIZE); // initialize the charBuffer[i] to all 0s
		charCount[i] = 0;
	}
	for(i = 0; i < PROGRAM_COUNT; i++){
		freez[i] = -1;
	}
  
  ctlFLAG = 0;
  shiftFLAG = 0;
  altFLAG = 0;
  cmdFLAG = 0;
  CapsLockFLAG = 0;
  kb_enable_irq();
}

/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"
// #include <linux/spinlock.h>

//static spinlock_t PIC_lock = SPIN_LOCK_UNLOCKED;
/* Interrupt masks to determine which interrupts are enabled and disabled */
static uint8_t master_mask = 0xff; /* IRQs 0-7  */
static uint8_t slave_mask = 0xff;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
  // unsigned long flags;
  outb(master_mask,   MASTER_8259_PORT+1);
  outb(slave_mask,    SLAVE_8259_PORT+1);
  outb(ICW1,          MASTER_8259_PORT);
  outb(ICW2_MASTER,   MASTER_8259_PORT+1);
  outb(ICW3_MASTER,   MASTER_8259_PORT+1);
  outb(ICW4,          MASTER_8259_PORT+1);
  outb(ICW1,          SLAVE_8259_PORT);
  outb(ICW2_SLAVE,    SLAVE_8259_PORT+1);
  outb(ICW3_SLAVE,    SLAVE_8259_PORT+1);
  outb(ICW4,          SLAVE_8259_PORT+1);
  outb(master_mask,   MASTER_8259_PORT+1);
  outb(slave_mask,    SLAVE_8259_PORT+1);
  enable_irq(2);
}

/* Disnable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
  if(irq_num > 7){
    irq_num -= 8;
    slave_mask |= 1 << irq_num;
    outb(slave_mask, SLAVE_8259_PORT+1);
  }else{
    master_mask |= 1 << irq_num;
    outb(master_mask, MASTER_8259_PORT+1);
  }
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
  if(irq_num > 7){
    irq_num -= 8;
    slave_mask &= ~(1 << irq_num);
    outb(slave_mask, SLAVE_8259_PORT+1);
  }else{
    master_mask &= ~(1 << irq_num);
    outb(master_mask, MASTER_8259_PORT+1);
  }
  // spin_unlock_irqrestore(&PIC_lock, flags);
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
  if(irq_num >= ICW2_MASTER) irq_num -= ICW2_MASTER;
  //Determine if the irq goes to slave or master
  if(irq_num > 7){
      irq_num -= 8;
      outb(EOI | irq_num, SLAVE_8259_PORT);
  }else{
      outb(EOI | irq_num, MASTER_8259_PORT);
  }
}

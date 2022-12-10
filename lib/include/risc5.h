/*
 * risc5.h -- RISC5 specific declarations and constants
 */

#ifndef __RISC5_H
#define __RISC5_H

#include <stdint.h>

/*********************************************************************************************************************/
/*** Interrupts ***/

/* interrupt vector table */
extern void (*risc5_ivt[16])(void);  /* defined in crt0.s */

/* IRQ numbers */
#define RISC5_IRQ_HP_TIMER      (15U)
#define RISC5_IRQ_MS_TIMER      (11U)
#define RISC5_IRQ_RS232_0_RX    (7U)
#define RISC5_IRQ_RS232_0_TX    (6U)
#define RISC5_IRQ_BUTTONS       (3U)

/*********************************************************************************************************************/
/*** Devices ***/

/* Device base addresses */
#define RISC5_IO_BASE		    (0xFFFFC0U)
#define RISC5_XIO_BASE	        (0xFFFF80U)

/* Standard device addresses */
#define RISC5_IO_MS_TIMER	            ((volatile uint32_t*)((RISC5_IO_BASE) + (4*0)))
#define RISC5_IO_SWITCH                 ((volatile uint32_t*)((RISC5_IO_BASE) + (4*1)))
#define RISC5_IO_LED                    RISC5_IO_SWITCH /* SWITCH: read only, LED: write only */
#define RISC5_IO_SERIAL_DATA            ((volatile uint32_t*)((RISC5_IO_BASE) + (4*2)))
#define RISC5_IO_SERIAL_STAT_CTRL       ((volatile uint32_t*)((RISC5_IO_BASE) + (4*3)))
#define RISC5_IO_SPI_DATA               ((volatile uint32_t*)((RISC5_IO_BASE) + (4*4)))
#define RISC5_IO_SPI_STAT_CTRL          ((volatile uint32_t*)((RISC5_IO_BASE) + (4*5)))
#define RISC5_IO_MOUSE_DATA_KBD_STAT    ((volatile uint32_t*)((RISC5_IO_BASE) + (4*6)))
#define RISC5_IO_KBD_DATA               ((volatile uint32_t*)((RISC5_IO_BASE) + (4*7)))
#define RISC5_IO_GPIO_DATA              ((volatile uint32_t*)((RISC5_IO_BASE) + (4*8)))
#define RISC5_IO_GPIO_DIR               ((volatile uint32_t*)((RISC5_IO_BASE) + (4*9)))
#define RISC5_IO_SIM_SHUTDOWN           ((volatile uint32_t*)0xFFFFFC)

/* Extended device addresses */
#define RISC5_IO_HP_TIMER_DATA          ((volatile uint32_t*)((RISC5_XIO_BASE) + (4*0)))
#define RISC5_IO_HP_TIMER_CTRL          ((volatile uint32_t*)((RISC5_XIO_BASE) + (4*1)))
#define RISC5_IO_LCD_DATA               ((volatile uint32_t*)((RISC5_XIO_BASE) + (4*2)))
#define RISC5_IO_LCD_STAT_CTRL          ((volatile uint32_t*)((RISC5_XIO_BASE) + (4*3)))

/*********************************************************************************************************************/
/*** Cpu Intrinsics ***/

extern void _asm_CLI(void);
extern void _asm_STI(void);
extern void _asm_NOP(void);

#define RISC5_DISABLE_INTERRUPTS() _asm_CLI()
#define RISC5_ENABLE_INTERRUPTS() _asm_STI()
#define RISC5_NOP() _asm_NOP()

/*********************************************************************************************************************/
/*** Interrupt dispatch ***/

/*
 * The default interrupt dispatching behaviour can be overwritten
 * by writing a jump instruction to a user provided dispatcher at
 * address 4 (risc5_start_irq_dispatch).
 * 
 * To do so, the macro RISC5_SET_IRQ_DISPATCHER(dispatcher_adress) 
 * is provided, which correctly encodes the jump to the new dispatcher.
 * 
 * Example:
 *      extern void(*my_dispatcher)(void);
 *      uint32_t old_jump_instruction = risc5_start_irq_dispatch;
 *      RISC5_SET_IRQ_DISPATCHER(my_dispatcher);
 * 
 * NOTE: The dispatcher is not a normal function and should be written 
 *       in assembly language. See `risc5_default_irq_dispatcher` in
 *       crt0.s for a example dispatcher. 
 * NOTE: Changing the interrupt dispatcher should only be necessary 
 *       when writing very low level code like an an operating system. 
 *       Normal code wishing to install a interrupt service routine
 *       for a particular interrupt should do so by writing the address 
 *       of the isr to the corresponding entry in risc5_ivt[].
 */

extern uint32_t risc5_start_irq_dispatch;
extern uint32_t risc5_default_irq_dispatcher;

/* 
 * Decode address of current irq dispatcher from branch instruction at address risc5_start_irq_dispatch.
 * B Instruction: 
 *             4      4     2       22
 *      +------+------+------+------+------+------+------+------+
 * F3:  | 1110 | 0111 | 00      off                             |
 *      +------+------+------+------+------+------+------+------+
 * Destination address = PC + 4 + off * 4, where PC is the current program counter.
 */
#define RISC5_GET_IRQ_DISPATCHER() ((&risc5_start_irq_dispatch) + 1 + (risc5_start_irq_dispatch & 0x3fffff))

/* 
 * Write a branch instruction to dispatcher_adress at address risc5_start_irq_dispatch.
 */
#define RISC5_SET_IRQ_DISPATCHER(dispatcher_adress)                                                                 \
    do{                                                                                                             \
        risc5_start_irq_dispatch = (0xe7000000 | ((uint32_t *)(dispatcher_adress) - &risc5_start_irq_dispatch - 1));   \
    } while(0)

#endif /* __RISC5_H */

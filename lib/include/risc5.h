/*
 * risc5.h -- RISC5 specific declarations and constants
 */

#ifndef __RISC5_H
#define __RISC5_H

#include <stdint.h>

/*** Interrupts ***/

/* interrupt vector table */
extern uint32_t risc5_ivt[16];  /* defined in crt0.s */

/* IRQ numbers */
#define RISC5_IRQ_HP_TIMER      (15U)
#define RISC5_IRQ_MS_TIMER      (11U)
#define RISC5_IRQ_RS232_0_RX    (7U)
#define RISC5_IRQ_RS232_0_TX    (6U)
#define RISC5_IRQ_BUTTONS       (3U)

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

/* Extended device addresses */
#define RISC5_IO_HP_TIMER_DATA          ((volatile uint32_t*)((RISC5_XIO_BASE) + (4*0)))
#define RISC5_IO_HP_TIMER_CTRL          ((volatile uint32_t*)((RISC5_XIO_BASE) + (4*1)))
#define RISC5_IO_LCD_DATA               ((volatile uint32_t*)((RISC5_XIO_BASE) + (4*2)))
#define RISC5_IO_LCD_STAT_CTRL          ((volatile uint32_t*)((RISC5_XIO_BASE) + (4*3)))

/*** Cpu Intrinsics ***/

extern void _asm_CLI(void);
extern void _asm_STI(void);
extern void _asm_NOP(void);

#define RISC5_DISABLE_INTERRUPTS() _asm_CLI()
#define RISC5_ENABLE_INTERRUPTS() _asm_STI()
#define RISC5_NOP() _asm_NOP()

#endif /* __RISC5_H */

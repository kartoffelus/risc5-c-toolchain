/*
 * change interrupt dispatcher
 */

#include <risc5.h>

void assertFn( const char *fileName, int line );
#define Assert(expr) if (expr) {} else assertFn(__FILE__, __LINE__)

int main(void) {
    uint32_t old_start_irq_dispatch = risc5_start_irq_dispatch;
    
    uint32_t *current_irq_dispatcher = RISC5_GET_IRQ_DISPATCHER();
    Assert(current_irq_dispatcher == &risc5_default_irq_dispatcher);

    RISC5_SET_IRQ_DISPATCHER(&risc5_default_irq_dispatcher);
    Assert(risc5_start_irq_dispatch == old_start_irq_dispatch);

    current_irq_dispatcher = RISC5_GET_IRQ_DISPATCHER();
    Assert(current_irq_dispatcher == &risc5_default_irq_dispatcher);

    RISC5_SET_IRQ_DISPATCHER(0x1234);                   // Hypothetical irq dispatcher at address 0x1234
    Assert(risc5_start_irq_dispatch == 0xe700048B);     // 0xe7000000 | ((0x1234 - 4 - 4) / 4)

    current_irq_dispatcher = RISC5_GET_IRQ_DISPATCHER();
    Assert(current_irq_dispatcher == (uint32_t*)0x1234);

    *RISC5_IO_SIM_SHUTDOWN = 0x0;
    return 0;
}

void assertFn( const char *fileName, int line )
{
    RISC5_DISABLE_INTERRUPTS();
    ( void ) fileName;
    ( void ) line;
    *RISC5_IO_SIM_SHUTDOWN = 0x1;
}

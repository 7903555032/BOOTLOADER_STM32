#include "flash_layout.h"
#include "main.h"
#include "bl_jump.h"

typedef void (*pFunction)(void);

void JumpToApplication(void)
{
    uint32_t appStack;
    uint32_t appResetHandler;
    pFunction appEntry;

    appStack = *(volatile uint32_t *)APP_START_ADDR;

    appResetHandler = *(volatile uint32_t *)(APP_START_ADDR + 4);

    appEntry = (pFunction)appResetHandler;

    __disable_irq();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    SCB->VTOR = APP_START_ADDR;

    __set_MSP(appStack);

    appEntry();
}

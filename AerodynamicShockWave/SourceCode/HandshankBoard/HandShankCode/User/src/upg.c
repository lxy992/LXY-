
#include "config.h"

uint8_t UPG_Buf[4] = {0};
uint8_t UPG_ProcTime=0;

bool UPG_Proc(void)
{
    if(UPG_Buf[0] != 'U') return FALSE;
    if(UPG_Buf[1] != 'P') return FALSE;
    if(UPG_Buf[2] != 'G') return FALSE;
    if(UPG_Buf[3] != 'R') return FALSE;
    return TRUE;
}

void ClearBuffer(void)
{
    for(uint8_t i = 0; i < 4; i++)
    {
        UPG_Buf[i] = 0x00;
    }
}

void Jump_ApplicationArea(void)
{
    //jump to GO address
    asm("LDW X,  SP ");
    asm("LD  A,  $FF");
    asm("LD  XL, A  ");
    asm("LDW SP, X  ");
    asm("JPF $8000");
}

void UPG_Reset(void)
{
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    /* Program complement value (of previous read byte) at previous address + 1 */
    FLASH_ProgramByte((0x4050), 'U');
    while(FLASH_GetFlagStatus(FLASH_FLAG_EOP)==RESET);
    /*Lock Data memory */
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    WWDG_SWReset();
}



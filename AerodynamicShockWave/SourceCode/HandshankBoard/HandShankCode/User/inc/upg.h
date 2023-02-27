
#ifndef _UPG_H
#define _UPG_H

#define MCU_UPG_PROC

extern uint8_t UPG_Buf[4];
extern uint8_t UPG_ProcTime;

bool UPG_Proc(void);
void ClearBuffer(void);
void UPG_Reset(void);

#endif


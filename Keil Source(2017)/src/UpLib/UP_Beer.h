#ifdef  UP_Beer_H
#define UP_Beer_H

extern void UP_Beer_Set(u8 Channel,u32 Rate,u8 Value);
extern u32 g_Timer2Count;
void TimerHadler2(u32 timerchannel);
void UP_Beer_Delay(u32 nCount);
#endif
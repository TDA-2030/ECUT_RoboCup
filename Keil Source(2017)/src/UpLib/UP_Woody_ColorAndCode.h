#ifndef _UP_WoodyColorAndCode_H_
#define _UP_WoodyColorAndCode_H_
extern void UP_Woody_StartUp(u32 command);
extern void UP_Woody_ColorRecLetIssue(void);
extern void UP_Woody_ColorRecForbidIssue(void);
extern u16 UP_Woody_ColorRec_ID_Judge(void);
extern void UP_Woody_Display_ColorID(void);

extern void UP_Woody_BarCodeRecLetIssue(void);
extern void UP_Woody_BarCodeRecForbidIssue(void);
extern u16 UP_Woody_BarCodeRec_ID_Judge(void);
extern void UP_Woody_Display_BarCodeID(void);

extern void UP_Woody_QrCodeRecLetIssue(void);
extern void UP_Woody_QrCodeRecForbidIssue(void);
extern u16 UP_Woody_QrCodeRec_ID_Judge(void);
extern void UP_Woody_Display_QrCodeID(void);
#endif


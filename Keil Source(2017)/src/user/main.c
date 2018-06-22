
//东华理工大学2017年武术擂台赛程序


#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\dip_angle.h"
#include "..\user\forward.h"
#include "..\user\find_enemy.h"
#include "..\user\forward_attack.h"


//全局变量
SYS_STATE_e SYS_State=SYS_NORMAL; //系统状态变量

void come_to_power(void);    //上台


#define WATCH_PARAMETER 0


int main(void)
{
	machine_init();  //初始化
	
	come_to_power();    //上台
	UP_LCD_ClearScreen();
	
	while(1)
	{
		dip_angle();
		
		if(SYS_State!=SYS_FOUND)
		{
			Set_LED(LED_PINK);
			find_enemy();
		}
		if(SYS_State==SYS_FOUND)
		{
			Set_LED(LED_RED);
			forward_attack();
		}
		if(SYS_State==SYS_NORMAL)
		{
			Set_LED(LED_BLUE);
			forward();
		}
	#if (WATCH_PARAMETER==1)  //选择是否编译调试信息
		Get_sensor(SEN_ANGLE);
		Get_sensor(SEN_GRAY);
		Get_sensor(SEN_IR);
//		UP_LCD_ShowInt(0,0,Sen_Data.AD_advan);
//		UP_LCD_ShowInt(0,1,Sen_Data.AD_back);
//		UP_LCD_ShowInt(0,2,Sen_Data.AD_left);
//		UP_LCD_ShowInt(0,3,Sen_Data.AD_right);
//		UP_LCD_ShowInt(5,0,Sen_Data.AD_X_angle);
//		UP_LCD_ShowInt(5,1,Sen_Data.AD_Y_angle);
		
		UP_LCD_ShowInt(0,0,Sen_Data.inf_A);
		UP_LCD_ShowInt(2,0,Sen_Data.inf_B);
		
		UP_LCD_ShowInt(0,1,Sen_Data.inf_LA);
		UP_LCD_ShowInt(2,1,Sen_Data.inf_RA);
		
		UP_LCD_ShowInt(0,2,Sen_Data.inf_L);
		UP_LCD_ShowInt(2,2,Sen_Data.inf_R);
		
		UP_LCD_ShowInt(0,2,INF_ULA);
		UP_LCD_ShowInt(2,2,INF_URA);
		UP_delay_ms(100);
	#endif
	}
}


void come_to_power()    //上台
{
	Set_motor(0,0,2);
	Set_servo(800,548,592,512,512,396,455,250,
			  400,530,200,400,350,200,530,400,
			  10);
	while(1)
	{
		if ((INF_L==0 && INF_R==0))  //检测到为0 未检测到为1
		{
			break;
		}
		UP_delay_ms(100);//为了消除小于100ms的偶然干扰
	}
	Set_servo(800,548,592,150,839,396,455,250,
			  400,530,200,450,450,200,530,400,
			  800);
	Speed_up(200,500,MOT_ALL,500);
	Set_motor(470,400,1000);
	default_act(10);  		//默认动作
	Set_motor(450,250,800);    //右转
}

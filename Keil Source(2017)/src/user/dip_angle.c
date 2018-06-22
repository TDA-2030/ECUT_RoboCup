

/* ------------  头文件  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\dip_angle.h"
#include "..\user\forward.h"

//倾角检测函数
//多方面综合判断机器人倾倒情  判断前翻和后翻   及左翻 右翻
//确认机器人已经倒了则执行相应动作，使机器人自行爬起，并相应设置系统状态SYS_State
//要求：准确的判断机器人倾倒情况，不能误判也不能检测不到倾倒，不要求快速性
int dip_angle(void)
{
	int i;
    //判断倾倒
	Get_sensor(SEN_ANGLE); //读取角度数据
	//往后倒了
	if(Sen_Data.AD_Y_angle>(MAX_Angle-60)) //第一次放宽判断条件
	{
		UP_delay_ms(5);
		Get_sensor(SEN_ANGLE);//Get_sensor(SEN_GRAY);
		if((Sen_Data.AD_Y_angle>MAX_Angle)&&(INF_B==0))//角度，红外后，灰度综合检测
		{
			SYS_State = SYS_DUMP;
			Set_motor(0,0,1);
			back_fall();
			out_flag=0;out_time=0;SYS_State=SYS_NORMAL;
			//UP_Set_Beer(1500,1);
			Set_motor(420,420,2);
			return 1;
		}
	}
	//往前倒了
	else if(Sen_Data.AD_Y_angle<(MIN_Angle+60)) //第一次放宽判断条件
	{
		UP_delay_ms(5);
		Get_sensor(SEN_ANGLE);Get_sensor(SEN_GRAY);
		if(Sen_Data.AD_Y_angle<MIN_Angle&&INF_A==0)
	  //if(Sen_Data.AD_Y_angle<MIN_Angle&&INF_A==0)//角度，红外，灰度综合检测
		{
			SYS_State = SYS_DUMP;
			Set_motor(0,0,1);
			farward_fall();
			out_flag=0;out_time=0;SYS_State=SYS_NORMAL;
			Set_motor(420,420,2);
			return 1;
		}
	}
	//往右倒了
	else if(Sen_Data.AD_X_angle>(MAX_Angle-80)) //第一次放宽判断条件
	{
		UP_delay_ms(2);
		Get_sensor(SEN_ANGLE);//Get_sensor(SEN_GRAY);
		if(Sen_Data.AD_X_angle>MAX_Angle&&INF_R==0&&Sen_Data.AD_ave<AD_GRAY_DIP)//角度，红外，灰度综合检测
		{
			SYS_State = SYS_DUMP;
			Set_motor(0,0,1);
			out_flag=0;out_time=0;SYS_State=SYS_NORMAL;
			Set_servo(488,480,638,0,0,512,225,518,
					  350,350,200,300,300,200,400,400,
					  1000);
	
			Set_servo(185,842,630,0,0,0,0,0,
					  350,300,200,300,300,200,200,200,
					  500);
			i=0;
			while(++i<1500)
			{
				Get_sensor(SEN_ANGLE);
				if(Sen_Data.AD_Y_angle>(MAX_Angle-70)||Sen_Data.AD_Y_angle<(MIN_Angle+70))
					if(dip_angle()) {SYS_State = SYS_NORMAL;break;} //递归调用
				UP_delay_ms(1);
			}
		}
	}
	//往左倒了
	else if(Sen_Data.AD_X_angle<(MIN_Angle+80)) //第一次放宽判断条件
	{
		UP_delay_ms(2);
		Get_sensor(SEN_ANGLE);
		if(Sen_Data.AD_X_angle<MIN_Angle&&INF_L==0&&Sen_Data.AD_ave<AD_GRAY_DIP)//角度，红外，灰度综合检测
		{
			SYS_State = SYS_DUMP;
			Set_motor(0,0,1);
			out_flag=0;out_time=0;SYS_State=SYS_NORMAL;
			Set_servo(489,808,538,0,0,330,533,545,
					  400,400,200,300,300,200,350,350,
					  1000);
			
			Set_servo(0,0,0,0,0,344,209,842,
					200,200,300,300,300,200,250,250,
					500);
			i=0;
			while(++i<1500)
			{
				Get_sensor(SEN_ANGLE);
				if(Sen_Data.AD_Y_angle>(MAX_Angle-70)||Sen_Data.AD_Y_angle<(MIN_Angle+70))
					if(dip_angle()) {SYS_State = SYS_NORMAL;break;}  //递归调用
				UP_delay_ms(1);
			}
		}
	}
	return 0;
}



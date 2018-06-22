

/* ------------  头文件  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\forward.h"


int  out_time=0;    	 //超边界次数
bool out_flag=0;		 //超边界标志


//按场地灰度行走函数
//根据灰度大小控制如何转向保持机器人在场地较中心的位置，转向具有加速控制
//行走轨迹为随机行走，大概每次经过场地中心
void forward(void)
{
	const float _P       = 0.50; //-----------转向加速比例控制
	const int tu_off     = 450;  //-----------转向原始速度
	const int speed_max  = 870;  //-----------转向加速最大速度限制
	const int speed_walk = 420;  //-----------行走速度控制
	static char stuck_time=0;
	if(Get_sensor(SEN_GRAY)==0)
	{
		int s;
		//超出了边
		if(Sen_Data.AD_ave<AD_GRAY_LINE)
		{
			if(out_flag==1&&SYS_State==SYS_NORMAL) //在正常状态才进行转向加速控制
				out_time++;/*对超边界次数计数*/
			else
				out_time=0;
			s=(int)((float)out_time*_P);//注意延时与比例的配合
			if(tu_off+s>speed_max)s=speed_max-tu_off;//限制最大速度
			if(out_flag==0)out_flag=1;
			
			if(INF_ULA==1||INF_URA==1)
			{
				if(++stuck_time>20)
				{
					stuck_time = 0;
					Speed_up(550,200,MOT_ALL,250);
					Speed_up(-200,-350,MOT_ALL,800);
				}
			}
			//左边靠外
			if((Sen_Data.AD_left<Sen_Data.AD_right))
			{
				Set_motor(tu_off+s,-200,2);
			}
			//右边靠外
			else if((Sen_Data.AD_right<Sen_Data.AD_left))
			{
				Set_motor(-200,tu_off+s,2);
			}
		}
		else
		{
			out_flag=0;out_time=0;
			Set_motor(speed_walk,speed_walk,2);
		}
	}
}


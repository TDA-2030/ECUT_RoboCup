

/* ------------  头文件  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\find_enemy.h"
#include "..\user\dip_angle.h"


enemy_state_e enemy_sta = ENEMY_NONE;

//寻找对手函数
//在没有找到对手时SYS_State值为SYS_NORMAL，主函数将调用此函数进行寻找
//利用前后左右的红外传感器检测机器人周围是否有对手，同时检测倾角传感器
//后面检测到敌人直接转弯逃跑，其他方向的敌人则转向寻找，直到前面直对敌人完成寻找过程，设置系统状态SYS_State为SYS_FOUND
//要求：以最快的速度完成寻找过程，并有效破解敌人先发现我们并进行攻击的情况
void find_enemy(void)
{
	u16 temp,s;
	static int find_time=0; //找对手时间
	const int wait_time=500;//--------检测等待时间
	const int offset=200;//------------转向加速量
	
	Get_sensor(SEN_IR);
/*-------------------------前面检测到-------------------------*/
	if(Sen_Data.inf_A==0) //6种有效情况
	{
		Set_motor(400,400,2);
		SYS_State=SYS_FOUND;   //表示找到对手
		enemy_sta = ENEMY_A;   //标记对手在正前方找到
		return;
	}//end of 前面检测到
/*-------------------------左前或右前检测到-------------------------*/
	else if((Sen_Data.inf_LA==0||Sen_Data.inf_RA==0))
	{
		u16 i=300; //转弯时间t=2*i(ms)
		SYS_State=SYS_FOUNDING;
		find_time=Sys_Time;
		if(INF_LA==0)//左前检测到
		{
			while(i--)
			{
				Set_motor(-200,500,2); //左转
				if(INF_A==0)
				{
					Set_motor(400,400,2);
					SYS_State=SYS_FOUND;   //表示找到对手
					enemy_sta = ENEMY_LA;   //标记对手在左前方找到
					return;
				}
			}
		}
		else if(INF_RA==0)//右前检测到
		{
			while(i--)
			{
				Set_motor(500,-200,2); //左转
				if(INF_A==0)
				{
					Set_motor(400,400,2);
					SYS_State=SYS_FOUND;   //表示找到对手
					enemy_sta = ENEMY_RA;   //标记对手在右前方找到
					return;
				}
			}
		}
		SYS_State=SYS_NORMAL;
	}//end of 左前或右前检测到
/*-------------------------左或右检测到-------------------------*/
	else if(Sen_Data.inf_L==0||Sen_Data.inf_R==0)
	{
		u16 i=600; //转弯时间t=2*i(ms)
		SYS_State=SYS_FOUNDING;
		if(INF_L==0)//左检测到
		{
			for(temp=0;temp<i;temp++)
			{
				s=temp/(i/offset);
				Set_motor(-(300+s),500+s,2);
				if(INF_A==0||INF_LA==0)
				{
					enemy_sta = ENEMY_L;   //标记对手在左方找到
					return;
				}
			}
		}
		else if(INF_R==0)//右检测到
		{
			for(temp=0;temp<wait_time;temp++)
			{
				s=temp/(wait_time/offset);
				Set_motor(500+s,-(300+s),2);
				if(INF_A==0||INF_RA==0)
				{
					enemy_sta = ENEMY_R;   //标记对手在右方找到
					return;
				}
			}
		}
		SYS_State=SYS_NORMAL;
	}//end of 左或右检测到
/*-------------------------后面检测到-------------------------*/
	else if(Sen_Data.inf_B==0)
	{
		Get_sensor(SEN_GRAY);
		//左边靠外
		if(Sen_Data.AD_ave>AD_GRAY_LINE+500) //在场地较中间
		{
			Set_motor(630,230,2);
			return;
		}
	}
//	if((INF_L==0)&&INF_B==1&&INF_R==1)  //左边检测到
//	{
//		SYS_State=SYS_FOUNDING;
//		for(temp=0;temp<wait_time;temp++)
//		{
//			if(INF_RA==0||INF_R==0)//转太多，右边检测到了
//			{
//				Set_motor(500,-500,100);
//				if(INF_A==0)
//				{
//					SYS_State=SYS_FOUND;   //表示找到对手
//					Set_motor(300,300,2);
//				}
//				else 
//				{
//					SYS_State=SYS_NORMAL; //表示没有找到对手
//					return;
//				}
//				
//			}
			
//			if(INF_A==0)
//			{
//				SYS_State=SYS_FOUND;   //表示找到对手
//				Set_motor(300,300,2);
//				return;
//			}
//			else
//			{
//				if(temp%100==0){if(dip_angle())return;}//检测到倒下则退回到主函数重新开始
//				s=temp/(wait_time/offset);
//				Set_motor(-(300+s),500+s,2);
//			}
//		}
//		Set_motor(300,300,2);
//		SYS_State=SYS_NORMAL; //耗完时间表示没有找到对手
//	}
//	else if((INF_R==0||INF_RA==0)&&INF_A==1&&INF_B==1&&INF_L==1)  //右边检测到
//	{
//		SYS_State=SYS_FOUNDING;
//		for(temp=0;temp<wait_time;temp++)
//		{
//			if(INF_LA==0||INF_L==0)//转太多，左边检测到了
//			{
//				Set_motor(-500,500,100);
//				if(INF_A==0)
//				{
//					SYS_State=SYS_FOUND;   //表示找到对手
//					Set_motor(300,300,3);
//				}
//				else
//				{
//					SYS_State=SYS_NORMAL; //表示没有找到对手
//					return;
//				}
//			}
//			
//			if(INF_A==0)
//			{
//				SYS_State=SYS_FOUND;   //表示找到对手
//				Set_motor(300,300,3);
//				return;
//			}
//			else
//			{
//				if(temp%100==0){if(dip_angle())return;}//检测到倒下则退回到主函数重新开始
//				s=temp/(wait_time/offset);
//				Set_motor(500+s,-(300+s),3);
//			}
//		}
//		SYS_State=SYS_NORMAL; //耗完时间表示没有找到对手
//	}
	// else if(INF_A==1&&INF_B==0&&INF_L==1&&INF_R==1)  //后面检测到
	// {
		// Get_sensor(SEN_GRAY);
		//左边靠外
		// if(Sen_Data.AD_left<Sen_Data.AD_right)
		// {
			// Set_motor(630,230,1);
			// for(temp=0;temp<wait_time;temp++)
			// {
				// Get_sensor(SEN_IR);
				// if(INF_A==0||INF_L==0||INF_R==0)
				// {
					// break;
				// }
			// }
		// }
		//右边靠外
		// else if(Sen_Data.AD_right<Sen_Data.AD_left)
		// {
			// Set_motor(230,630,1);
			// for(temp=0;temp<wait_time;temp++)
			// {
				// Get_sensor(SEN_IR);
				// if(INF_A==0||INF_L==0||INF_R==0)
				// {
					// break;
				// }
			// }
		// }
	// }
	
}


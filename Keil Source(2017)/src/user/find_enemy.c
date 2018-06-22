

/* ------------  头文件  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\find_enemy.h"
#include "..\user\dip_angle.h"


enemy_state_e enemy_sta = ENEMY_NONE;


#define FIND_FAIL() {find_sta=0;enemy_dir=ENEMY_NONE;SYS_State=SYS_NORMAL;}


//寻找对手函数
//在没有找到对手时SYS_State值为SYS_NORMAL，主函数将调用此函数进行寻找
//利用前后左右的红外传感器检测机器人周围是否有对手，同时检测倾角传感器
//后面检测到敌人直接转弯逃跑，其他方向的敌人则转向寻找，直到前面直对敌人完成寻找过程，设置系统状态SYS_State为SYS_FOUND
//要求：以最快的速度完成寻找过程，并有效破解敌人先发现我们并进行攻击的情况
void find_enemy(void)
{
	u32 temp;//记录找到的时间
	u32 s;
	static u8 find_sta=0; //找对手状态，函数内部使用
	static enemy_state_e enemy_dir=ENEMY_NONE; //对手所在方向
	static u32 find_time=0; //标记第一次发现对手的时间
	static u32 LRA_find_time=0;//标记前左或前右第一次发现对手的时间
	static bool LRA_find_flag=0;//标记前左或前右发现了对手，确保LRA_find_time为第一次发现的时间
	const u16 attack_time=2500;//--------左前或右前超时启动攻击时间
	const u16 wait_time1=800;//--------左前或右前检测等待时间
	const u16 wait_time2=1800;//--------左或右检测等待时间
	const u32 offset=200;//------------转向加速量

	Get_sensor(SEN_IR);
	switch(find_sta)
	{
		case 0://--------------------第一次寻找方向态
		{
			/*--------前面检测到--------*/
			if(Sen_Data.inf_A==0) //6种有效情况
			{
				Set_motor(700,700,2);
				find_sta=0;            //完成一次找对手
				enemy_dir=ENEMY_NONE;  //回到初始态
				SYS_State=SYS_FOUND;   //表示找到对手
				enemy_sta = ENEMY_A;   //标记对手在正前方找到
			}//end of 前面检测到
			/*---------左前或右前检测到-------*/
			else if((Sen_Data.inf_LA==0||Sen_Data.inf_RA==0))
			{
				SYS_State=SYS_FOUNDING;
				if(INF_LA==0)
					enemy_dir = ENEMY_LA;   //标记对手在左前方找到
				else if(INF_RA==0)
					enemy_dir = ENEMY_RA;   //标记对手在右前方找到
				find_time = Sys_Time; //记录找到的时间
				if(LRA_find_flag==0)LRA_find_time=find_time;//第一次发现则标记时间
				LRA_find_flag=1; //标记已经发现了
				find_sta=1; //下次进入第二步
				
			}//end of 左前或右前检测到
			/*--------左或右检测到---------*/
			else if(Sen_Data.inf_L==0||Sen_Data.inf_R==0)
			{
				SYS_State=SYS_FOUNDING;
				if(INF_L==0)
				{
					enemy_dir = ENEMY_L;   //标记对手在左方找到
					Set_servo(610,730,412,000,000,580,101,571,
							  400,530,200,200,200,200,500,500,
							  2);
				}
				else if(INF_R==0)
				{
					enemy_dir = ENEMY_R;   //标记对手在右方找到
					Set_servo(387,945,412,000,000,580,272,392,
							  500,500,200,200,200,200,530,400,
							  2);
				}
				find_time = Sys_Time; //记录找到的时间
				find_sta=1; //下次进入第二步
			}//end of 左或右检测到
		}break;
		case 1://--------------------第二次转向寻找态
		{
			switch(enemy_dir)
			{
				case ENEMY_LA:
				{
					if(INF_A==0||(INF_LA==0&&(Sys_Time>(attack_time+LRA_find_time)))) //左前或右前发现超时直接攻击
					{
						find_sta=2;//进入第三步
						Set_motor(300,300,2);
						LRA_find_flag=0;
						enemy_sta = ENEMY_LA; //标记对手在左前方找到
					}
					if(INF_LA==1)LRA_find_flag=0; //没挡到则恢复
					temp = Sys_Time-find_time;//从发现到当前的时间
					Set_motor(-450,450,2); //左转
					if(temp>wait_time1||INF_R==0||INF_RA==0) //转弯超时或错误
						FIND_FAIL();  //找对手失败，系统回到正常状态
				}break;
				case ENEMY_RA:
				{
					if(INF_A==0||(INF_RA==0&&(Sys_Time>(attack_time+LRA_find_time))))//左前或右前发现超时直接攻击
					{
						find_sta=2;//进入第三步
						Set_motor(300,300,2);
						LRA_find_flag=0;
						enemy_sta = ENEMY_RA;
					}
					if(INF_RA==1)LRA_find_flag=0;//没挡到则恢复
					temp = Sys_Time-find_time;//从发现到当前的时间
					Set_motor(450,-450,2); //右转
					if(temp>wait_time1||INF_L==0||INF_LA==0) //转弯超时或错误
						FIND_FAIL();  //找对手失败，系统回到正常状态
				}break;
				case ENEMY_L:
				{
					if(INF_A==0)
					{
						find_sta=2; //进入第三步
						Set_motor(300,300,2);
						enemy_sta = ENEMY_L;
					}
					temp = Sys_Time-find_time;//从发现到当前的时间
					if(temp>800)
					{
						Set_servo(000,000,000,000,000,584,123,247,
								  400,530,200,200,200,200,500,500,
								  2);
					}
					s=((temp*offset)/wait_time2);
					Set_motor(-(300-s),550-s,2); //左转
					if(temp>wait_time2||INF_R==0||INF_RA==0) //转弯超时或错误
					{
						FIND_FAIL();  //找对手失败，系统回到正常状态
						default_act(2);
					}
				}break;
				case ENEMY_R:
				{
					if(INF_A==0)
					{
						find_sta=2;//进入第三步
						Set_motor(300,300,2);
						enemy_sta = ENEMY_R;
					}
					temp = Sys_Time-find_time;//从发现到当前的时间
					if(temp>800)
					{
						Set_servo(741,916,397,000,000,000,000,000,
								  400,530,200,200,200,200,500,500,
								  2);
					}
					s=((temp*offset)/wait_time2);
					Set_motor(550-s,-(300-s),2); //右转
					if(temp>wait_time2||INF_L==0||INF_LA==0) //转弯超时或错误
					{
						FIND_FAIL();  //找对手失败，系统回到正常状态
						default_act(2);
					}
				}break;
				default:break;
			}
		}break;
		case 2://------------------------第三次寻找到态
		{
			Set_motor(300,300,2);
			find_sta=0;            //完成一次找对手
			LRA_find_time = 0;
			enemy_dir=ENEMY_NONE;  //回到初始态
			SYS_State=SYS_FOUND;   //表示找到对手
		}break;
		case 3://------------------------第四次寻找到态
		{
			find_sta=0; //回到初始态
			enemy_dir=ENEMY_NONE;//回到初始态
			SYS_State=SYS_NORMAL;//找对手失败系统回到正常状态
		}break;
		
		default:break;
		
	}
/*-------------------------后面检测到-------------------------*/
	if(Sen_Data.inf_B==0)
	{
		Get_sensor(SEN_GRAY);
		//左边靠外
		if(Sen_Data.AD_ave>AD_GRAY_LINE+150) //在场地较中间
		{
			Set_motor(1000,230,500);
			return;
		}
	}
}


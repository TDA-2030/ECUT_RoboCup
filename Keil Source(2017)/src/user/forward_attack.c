

/* ------------  ͷ�ļ�  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\forward_attack.h"
#include "..\user\forward.h"
#include "..\user\dip_angle.h"


//�������ֺ���
//���ҵ���������ǰ��ʱSYS_StateֵΪSYS_FOUND�������������ô˺������й���
//�ڹ���ʱ��ͬʱҪ������Ǽ�⡢�����Ƿ��¼�⡢̨�ӱ�Ե���
//Ҫ����ȷ�ж��쳣���������ϵͳ״̬SYS_State���˳�����
void forward_attack(void)
{
	u32 once_time;    //��ѭ����ʱ���
	u32 cycle_time=0; //ѭ����ʱ���
	u8  push_time=0;  //��ǰ��ʱ��
	u8 step=0;
	
	SYS_State = SYS_ATTACK;
	
	Set_servo(751,188,636,000,000,345,808,288,//������һ��׼��������˫�ֺ�£����
			  1000,850,700,300,300,700,850,1000,
			  2);
	
	Get_sensor(SEN_GRAY);
	if(Sen_Data.AD_ave<(AD_GRAY_LINE))
		Speed_up(300,650,MOT_ALL,200);
	else
		Speed_up(300,700,MOT_ALL,200);
		Set_motor(500,500,2);
	once_time=Sys_Time;
	while(1)
	{
		if(step==0&&(once_time+1100)<Sys_Time) //�����ڶ�������̧����
		{
			step=1;
			Set_servo(000,000,718,000,000,264,000,000,//˫������
					  200,200,300,300,300,300,200,200,
					  2);
			cycle_time=Sys_Time;
		}
		
		if(step==1&&(cycle_time+600)<Sys_Time)//�������������·ţ�ǰ��
		{
			step=2;
			Set_servo(600,284,664,000,000,316,718,386,//
					  1000,300,700,300,300,700,300,1000,
					  2);
			cycle_time=Sys_Time;
		}
		if(step==2&&(cycle_time+600)<Sys_Time)//�������ĸ���ǰ��
		{
			
			Set_servo(751,188,664,000,000,316,808,288,//˫�����ƶ���
					  1000,1000,700,300,300,700,1000,1000,
					  2);
			cycle_time=Sys_Time;
			if(++push_time>6){push_time=0;step=3;}
			else step=1;
		}
		
		if(step==3&&(cycle_time+1600)<Sys_Time)//����
		{
			step=4;
			Set_servo(301,435,662,000,000,318,572,721,//˫�ֶԱ�����
					  1000,300,700,300,300,700,300,1000,
					  2);
			cycle_time=Sys_Time;
		}
		if(step==4&&(cycle_time+1600)<Sys_Time)//����
		{
			step=5;
			Set_servo(275,760,667,000,000,319,209,733,//˫����ȫ�ſ�����
					  1000,300,700,300,300,700,300,1000,
					  2);
			cycle_time=Sys_Time;
		}
		if(step==5&&(cycle_time+1600)<Sys_Time)//����
		{
			step=6;
			Set_servo(418,187,912,000,000,149,752,606,//˫�����´�������׼������ѹ����
					  700,500,700,300,300,700,400,600,
					  2);
			cycle_time=Sys_Time;
		}
		
		if(step==6&&(cycle_time+1600)<Sys_Time)//����
		{
			step=7;
			Set_servo(000,000,664,000,000,386,000,000,//����ѹ����
					  100,300,700,300,300,700,300,100,
					  2);
			cycle_time=Sys_Time;
		}
		
		if(step==7&&(cycle_time+1600)<Sys_Time)//����
		{
			step=0;
			Set_servo(751,188,636,000,000,385,808,288,//�ص�������һ��׼��������˫�ֺ�£����
					  600,400,700,300,300,700,500,700,
					  2);
			cycle_time=Sys_Time;
		}
		
		
		
		
		if(Sys_Time%500==0){if(dip_angle())return;}//��⵽�������˻ص����������¿�ʼ
		
		if((INF_A==1&&INF_LA==1&&INF_RA==1))//�˳���������
		{
			UP_delay_ms(2);
			if((INF_A==1&&INF_LA==1&&INF_RA==1))
			{
				default_act(1);
				Set_motor(200,200,2);
				SYS_State=SYS_NORMAL;
				out_flag=0;
				return;
			}
		}
		if((INF_ULA==1&&INF_URA==1)&&(INF_A==1&&INF_LA==1&&INF_RA==1))//�����������
		{
			UP_delay_ms(5);
			Get_sensor(SEN_GRAY);
			if(INF_ULA==1&&INF_URA==1) //GRAY_EDGA��ʾ̨����ڵĵط��ĻҶ�ֵ
			{
				default_act(1);
				Speed_up(550,200,MOT_ALL,250);
				Speed_up(-200,-350,MOT_ALL,800);
				SYS_State=SYS_NORMAL;
				out_flag=0;
			}
		}
	}

}


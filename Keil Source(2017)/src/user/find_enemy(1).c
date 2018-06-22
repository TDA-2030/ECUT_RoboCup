

/* ------------  ͷ�ļ�  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\find_enemy.h"
#include "..\user\dip_angle.h"


enemy_state_e enemy_sta = ENEMY_NONE;

//Ѱ�Ҷ��ֺ���
//��û���ҵ�����ʱSYS_StateֵΪSYS_NORMAL�������������ô˺�������Ѱ��
//����ǰ�����ҵĺ��⴫��������������Χ�Ƿ��ж��֣�ͬʱ�����Ǵ�����
//�����⵽����ֱ��ת�����ܣ���������ĵ�����ת��Ѱ�ң�ֱ��ǰ��ֱ�Ե������Ѱ�ҹ��̣�����ϵͳ״̬SYS_StateΪSYS_FOUND
//Ҫ���������ٶ����Ѱ�ҹ��̣�����Ч�ƽ�����ȷ������ǲ����й��������
void find_enemy(void)
{
	u16 temp,s;
	static int find_time=0; //�Ҷ���ʱ��
	const int wait_time=500;//--------���ȴ�ʱ��
	const int offset=200;//------------ת�������
	
	Get_sensor(SEN_IR);
/*-------------------------ǰ���⵽-------------------------*/
	if(Sen_Data.inf_A==0) //6����Ч���
	{
		Set_motor(400,400,2);
		SYS_State=SYS_FOUND;   //��ʾ�ҵ�����
		enemy_sta = ENEMY_A;   //��Ƕ�������ǰ���ҵ�
		return;
	}//end of ǰ���⵽
/*-------------------------��ǰ����ǰ��⵽-------------------------*/
	else if((Sen_Data.inf_LA==0||Sen_Data.inf_RA==0))
	{
		u16 i=300; //ת��ʱ��t=2*i(ms)
		SYS_State=SYS_FOUNDING;
		find_time=Sys_Time;
		if(INF_LA==0)//��ǰ��⵽
		{
			while(i--)
			{
				Set_motor(-200,500,2); //��ת
				if(INF_A==0)
				{
					Set_motor(400,400,2);
					SYS_State=SYS_FOUND;   //��ʾ�ҵ�����
					enemy_sta = ENEMY_LA;   //��Ƕ�������ǰ���ҵ�
					return;
				}
			}
		}
		else if(INF_RA==0)//��ǰ��⵽
		{
			while(i--)
			{
				Set_motor(500,-200,2); //��ת
				if(INF_A==0)
				{
					Set_motor(400,400,2);
					SYS_State=SYS_FOUND;   //��ʾ�ҵ�����
					enemy_sta = ENEMY_RA;   //��Ƕ�������ǰ���ҵ�
					return;
				}
			}
		}
		SYS_State=SYS_NORMAL;
	}//end of ��ǰ����ǰ��⵽
/*-------------------------����Ҽ�⵽-------------------------*/
	else if(Sen_Data.inf_L==0||Sen_Data.inf_R==0)
	{
		u16 i=600; //ת��ʱ��t=2*i(ms)
		SYS_State=SYS_FOUNDING;
		if(INF_L==0)//���⵽
		{
			for(temp=0;temp<i;temp++)
			{
				s=temp/(i/offset);
				Set_motor(-(300+s),500+s,2);
				if(INF_A==0||INF_LA==0)
				{
					enemy_sta = ENEMY_L;   //��Ƕ��������ҵ�
					return;
				}
			}
		}
		else if(INF_R==0)//�Ҽ�⵽
		{
			for(temp=0;temp<wait_time;temp++)
			{
				s=temp/(wait_time/offset);
				Set_motor(500+s,-(300+s),2);
				if(INF_A==0||INF_RA==0)
				{
					enemy_sta = ENEMY_R;   //��Ƕ������ҷ��ҵ�
					return;
				}
			}
		}
		SYS_State=SYS_NORMAL;
	}//end of ����Ҽ�⵽
/*-------------------------�����⵽-------------------------*/
	else if(Sen_Data.inf_B==0)
	{
		Get_sensor(SEN_GRAY);
		//��߿���
		if(Sen_Data.AD_ave>AD_GRAY_LINE+500) //�ڳ��ؽ��м�
		{
			Set_motor(630,230,2);
			return;
		}
	}
//	if((INF_L==0)&&INF_B==1&&INF_R==1)  //��߼�⵽
//	{
//		SYS_State=SYS_FOUNDING;
//		for(temp=0;temp<wait_time;temp++)
//		{
//			if(INF_RA==0||INF_R==0)//ת̫�࣬�ұ߼�⵽��
//			{
//				Set_motor(500,-500,100);
//				if(INF_A==0)
//				{
//					SYS_State=SYS_FOUND;   //��ʾ�ҵ�����
//					Set_motor(300,300,2);
//				}
//				else 
//				{
//					SYS_State=SYS_NORMAL; //��ʾû���ҵ�����
//					return;
//				}
//				
//			}
			
//			if(INF_A==0)
//			{
//				SYS_State=SYS_FOUND;   //��ʾ�ҵ�����
//				Set_motor(300,300,2);
//				return;
//			}
//			else
//			{
//				if(temp%100==0){if(dip_angle())return;}//��⵽�������˻ص����������¿�ʼ
//				s=temp/(wait_time/offset);
//				Set_motor(-(300+s),500+s,2);
//			}
//		}
//		Set_motor(300,300,2);
//		SYS_State=SYS_NORMAL; //����ʱ���ʾû���ҵ�����
//	}
//	else if((INF_R==0||INF_RA==0)&&INF_A==1&&INF_B==1&&INF_L==1)  //�ұ߼�⵽
//	{
//		SYS_State=SYS_FOUNDING;
//		for(temp=0;temp<wait_time;temp++)
//		{
//			if(INF_LA==0||INF_L==0)//ת̫�࣬��߼�⵽��
//			{
//				Set_motor(-500,500,100);
//				if(INF_A==0)
//				{
//					SYS_State=SYS_FOUND;   //��ʾ�ҵ�����
//					Set_motor(300,300,3);
//				}
//				else
//				{
//					SYS_State=SYS_NORMAL; //��ʾû���ҵ�����
//					return;
//				}
//			}
//			
//			if(INF_A==0)
//			{
//				SYS_State=SYS_FOUND;   //��ʾ�ҵ�����
//				Set_motor(300,300,3);
//				return;
//			}
//			else
//			{
//				if(temp%100==0){if(dip_angle())return;}//��⵽�������˻ص����������¿�ʼ
//				s=temp/(wait_time/offset);
//				Set_motor(500+s,-(300+s),3);
//			}
//		}
//		SYS_State=SYS_NORMAL; //����ʱ���ʾû���ҵ�����
//	}
	// else if(INF_A==1&&INF_B==0&&INF_L==1&&INF_R==1)  //�����⵽
	// {
		// Get_sensor(SEN_GRAY);
		//��߿���
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
		//�ұ߿���
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


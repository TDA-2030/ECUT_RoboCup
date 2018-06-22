

/* ------------  ͷ�ļ�  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\find_enemy.h"
#include "..\user\dip_angle.h"


enemy_state_e enemy_sta = ENEMY_NONE;


#define FIND_FAIL() {find_sta=0;enemy_dir=ENEMY_NONE;SYS_State=SYS_NORMAL;}


//Ѱ�Ҷ��ֺ���
//��û���ҵ�����ʱSYS_StateֵΪSYS_NORMAL�������������ô˺�������Ѱ��
//����ǰ�����ҵĺ��⴫��������������Χ�Ƿ��ж��֣�ͬʱ�����Ǵ�����
//�����⵽����ֱ��ת�����ܣ���������ĵ�����ת��Ѱ�ң�ֱ��ǰ��ֱ�Ե������Ѱ�ҹ��̣�����ϵͳ״̬SYS_StateΪSYS_FOUND
//Ҫ���������ٶ����Ѱ�ҹ��̣�����Ч�ƽ�����ȷ������ǲ����й��������
void find_enemy(void)
{
	u32 temp;//��¼�ҵ���ʱ��
	u32 s;
	static u8 find_sta=0; //�Ҷ���״̬�������ڲ�ʹ��
	static enemy_state_e enemy_dir=ENEMY_NONE; //�������ڷ���
	static u32 find_time=0; //��ǵ�һ�η��ֶ��ֵ�ʱ��
	static u32 LRA_find_time=0;//���ǰ���ǰ�ҵ�һ�η��ֶ��ֵ�ʱ��
	static bool LRA_find_flag=0;//���ǰ���ǰ�ҷ����˶��֣�ȷ��LRA_find_timeΪ��һ�η��ֵ�ʱ��
	const u16 attack_time=2500;//--------��ǰ����ǰ��ʱ��������ʱ��
	const u16 wait_time1=800;//--------��ǰ����ǰ���ȴ�ʱ��
	const u16 wait_time2=1800;//--------����Ҽ��ȴ�ʱ��
	const u32 offset=200;//------------ת�������

	Get_sensor(SEN_IR);
	switch(find_sta)
	{
		case 0://--------------------��һ��Ѱ�ҷ���̬
		{
			/*--------ǰ���⵽--------*/
			if(Sen_Data.inf_A==0) //6����Ч���
			{
				Set_motor(700,700,2);
				find_sta=0;            //���һ���Ҷ���
				enemy_dir=ENEMY_NONE;  //�ص���ʼ̬
				SYS_State=SYS_FOUND;   //��ʾ�ҵ�����
				enemy_sta = ENEMY_A;   //��Ƕ�������ǰ���ҵ�
			}//end of ǰ���⵽
			/*---------��ǰ����ǰ��⵽-------*/
			else if((Sen_Data.inf_LA==0||Sen_Data.inf_RA==0))
			{
				SYS_State=SYS_FOUNDING;
				if(INF_LA==0)
					enemy_dir = ENEMY_LA;   //��Ƕ�������ǰ���ҵ�
				else if(INF_RA==0)
					enemy_dir = ENEMY_RA;   //��Ƕ�������ǰ���ҵ�
				find_time = Sys_Time; //��¼�ҵ���ʱ��
				if(LRA_find_flag==0)LRA_find_time=find_time;//��һ�η�������ʱ��
				LRA_find_flag=1; //����Ѿ�������
				find_sta=1; //�´ν���ڶ���
				
			}//end of ��ǰ����ǰ��⵽
			/*--------����Ҽ�⵽---------*/
			else if(Sen_Data.inf_L==0||Sen_Data.inf_R==0)
			{
				SYS_State=SYS_FOUNDING;
				if(INF_L==0)
				{
					enemy_dir = ENEMY_L;   //��Ƕ��������ҵ�
					Set_servo(610,730,412,000,000,580,101,571,
							  400,530,200,200,200,200,500,500,
							  2);
				}
				else if(INF_R==0)
				{
					enemy_dir = ENEMY_R;   //��Ƕ������ҷ��ҵ�
					Set_servo(387,945,412,000,000,580,272,392,
							  500,500,200,200,200,200,530,400,
							  2);
				}
				find_time = Sys_Time; //��¼�ҵ���ʱ��
				find_sta=1; //�´ν���ڶ���
			}//end of ����Ҽ�⵽
		}break;
		case 1://--------------------�ڶ���ת��Ѱ��̬
		{
			switch(enemy_dir)
			{
				case ENEMY_LA:
				{
					if(INF_A==0||(INF_LA==0&&(Sys_Time>(attack_time+LRA_find_time)))) //��ǰ����ǰ���ֳ�ʱֱ�ӹ���
					{
						find_sta=2;//���������
						Set_motor(300,300,2);
						LRA_find_flag=0;
						enemy_sta = ENEMY_LA; //��Ƕ�������ǰ���ҵ�
					}
					if(INF_LA==1)LRA_find_flag=0; //û������ָ�
					temp = Sys_Time-find_time;//�ӷ��ֵ���ǰ��ʱ��
					Set_motor(-450,450,2); //��ת
					if(temp>wait_time1||INF_R==0||INF_RA==0) //ת�䳬ʱ�����
						FIND_FAIL();  //�Ҷ���ʧ�ܣ�ϵͳ�ص�����״̬
				}break;
				case ENEMY_RA:
				{
					if(INF_A==0||(INF_RA==0&&(Sys_Time>(attack_time+LRA_find_time))))//��ǰ����ǰ���ֳ�ʱֱ�ӹ���
					{
						find_sta=2;//���������
						Set_motor(300,300,2);
						LRA_find_flag=0;
						enemy_sta = ENEMY_RA;
					}
					if(INF_RA==1)LRA_find_flag=0;//û������ָ�
					temp = Sys_Time-find_time;//�ӷ��ֵ���ǰ��ʱ��
					Set_motor(450,-450,2); //��ת
					if(temp>wait_time1||INF_L==0||INF_LA==0) //ת�䳬ʱ�����
						FIND_FAIL();  //�Ҷ���ʧ�ܣ�ϵͳ�ص�����״̬
				}break;
				case ENEMY_L:
				{
					if(INF_A==0)
					{
						find_sta=2; //���������
						Set_motor(300,300,2);
						enemy_sta = ENEMY_L;
					}
					temp = Sys_Time-find_time;//�ӷ��ֵ���ǰ��ʱ��
					if(temp>800)
					{
						Set_servo(000,000,000,000,000,584,123,247,
								  400,530,200,200,200,200,500,500,
								  2);
					}
					s=((temp*offset)/wait_time2);
					Set_motor(-(300-s),550-s,2); //��ת
					if(temp>wait_time2||INF_R==0||INF_RA==0) //ת�䳬ʱ�����
					{
						FIND_FAIL();  //�Ҷ���ʧ�ܣ�ϵͳ�ص�����״̬
						default_act(2);
					}
				}break;
				case ENEMY_R:
				{
					if(INF_A==0)
					{
						find_sta=2;//���������
						Set_motor(300,300,2);
						enemy_sta = ENEMY_R;
					}
					temp = Sys_Time-find_time;//�ӷ��ֵ���ǰ��ʱ��
					if(temp>800)
					{
						Set_servo(741,916,397,000,000,000,000,000,
								  400,530,200,200,200,200,500,500,
								  2);
					}
					s=((temp*offset)/wait_time2);
					Set_motor(550-s,-(300-s),2); //��ת
					if(temp>wait_time2||INF_L==0||INF_LA==0) //ת�䳬ʱ�����
					{
						FIND_FAIL();  //�Ҷ���ʧ�ܣ�ϵͳ�ص�����״̬
						default_act(2);
					}
				}break;
				default:break;
			}
		}break;
		case 2://------------------------������Ѱ�ҵ�̬
		{
			Set_motor(300,300,2);
			find_sta=0;            //���һ���Ҷ���
			LRA_find_time = 0;
			enemy_dir=ENEMY_NONE;  //�ص���ʼ̬
			SYS_State=SYS_FOUND;   //��ʾ�ҵ�����
		}break;
		case 3://------------------------���Ĵ�Ѱ�ҵ�̬
		{
			find_sta=0; //�ص���ʼ̬
			enemy_dir=ENEMY_NONE;//�ص���ʼ̬
			SYS_State=SYS_NORMAL;//�Ҷ���ʧ��ϵͳ�ص�����״̬
		}break;
		
		default:break;
		
	}
/*-------------------------�����⵽-------------------------*/
	if(Sen_Data.inf_B==0)
	{
		Get_sensor(SEN_GRAY);
		//��߿���
		if(Sen_Data.AD_ave>AD_GRAY_LINE+150) //�ڳ��ؽ��м�
		{
			Set_motor(1000,230,500);
			return;
		}
	}
}


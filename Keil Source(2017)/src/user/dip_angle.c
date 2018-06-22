

/* ------------  ͷ�ļ�  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\dip_angle.h"
#include "..\user\forward.h"

//��Ǽ�⺯��
//�෽���ۺ��жϻ������㵹��  �ж�ǰ���ͺ�   ���� �ҷ�
//ȷ�ϻ������Ѿ�������ִ����Ӧ������ʹ�������������𣬲���Ӧ����ϵͳ״̬SYS_State
//Ҫ��׼ȷ���жϻ������㵹�������������Ҳ���ܼ�ⲻ���㵹����Ҫ�������
int dip_angle(void)
{
	int i;
    //�ж��㵹
	Get_sensor(SEN_ANGLE); //��ȡ�Ƕ�����
	//������
	if(Sen_Data.AD_Y_angle>(MAX_Angle-60)) //��һ�ηſ��ж�����
	{
		UP_delay_ms(5);
		Get_sensor(SEN_ANGLE);//Get_sensor(SEN_GRAY);
		if((Sen_Data.AD_Y_angle>MAX_Angle)&&(INF_B==0))//�Ƕȣ�����󣬻Ҷ��ۺϼ��
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
	//��ǰ����
	else if(Sen_Data.AD_Y_angle<(MIN_Angle+60)) //��һ�ηſ��ж�����
	{
		UP_delay_ms(5);
		Get_sensor(SEN_ANGLE);Get_sensor(SEN_GRAY);
		if(Sen_Data.AD_Y_angle<MIN_Angle&&INF_A==0)
	  //if(Sen_Data.AD_Y_angle<MIN_Angle&&INF_A==0)//�Ƕȣ����⣬�Ҷ��ۺϼ��
		{
			SYS_State = SYS_DUMP;
			Set_motor(0,0,1);
			farward_fall();
			out_flag=0;out_time=0;SYS_State=SYS_NORMAL;
			Set_motor(420,420,2);
			return 1;
		}
	}
	//���ҵ���
	else if(Sen_Data.AD_X_angle>(MAX_Angle-80)) //��һ�ηſ��ж�����
	{
		UP_delay_ms(2);
		Get_sensor(SEN_ANGLE);//Get_sensor(SEN_GRAY);
		if(Sen_Data.AD_X_angle>MAX_Angle&&INF_R==0&&Sen_Data.AD_ave<AD_GRAY_DIP)//�Ƕȣ����⣬�Ҷ��ۺϼ��
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
					if(dip_angle()) {SYS_State = SYS_NORMAL;break;} //�ݹ����
				UP_delay_ms(1);
			}
		}
	}
	//������
	else if(Sen_Data.AD_X_angle<(MIN_Angle+80)) //��һ�ηſ��ж�����
	{
		UP_delay_ms(2);
		Get_sensor(SEN_ANGLE);
		if(Sen_Data.AD_X_angle<MIN_Angle&&INF_L==0&&Sen_Data.AD_ave<AD_GRAY_DIP)//�Ƕȣ����⣬�Ҷ��ۺϼ��
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
					if(dip_angle()) {SYS_State = SYS_NORMAL;break;}  //�ݹ����
				UP_delay_ms(1);
			}
		}
	}
	return 0;
}





/* ------------  ͷ�ļ�  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"
#include "..\user\forward.h"


int  out_time=0;    	 //���߽����
bool out_flag=0;		 //���߽��־


//�����ػҶ����ߺ���
//���ݻҶȴ�С�������ת�򱣳ֻ������ڳ��ؽ����ĵ�λ�ã�ת����м��ٿ���
//���߹켣Ϊ������ߣ����ÿ�ξ�����������
void forward(void)
{
	const float _P       = 0.50; //-----------ת����ٱ�������
	const int tu_off     = 450;  //-----------ת��ԭʼ�ٶ�
	const int speed_max  = 870;  //-----------ת���������ٶ�����
	const int speed_walk = 420;  //-----------�����ٶȿ���
	static char stuck_time=0;
	if(Get_sensor(SEN_GRAY)==0)
	{
		int s;
		//�����˱�
		if(Sen_Data.AD_ave<AD_GRAY_LINE)
		{
			if(out_flag==1&&SYS_State==SYS_NORMAL) //������״̬�Ž���ת����ٿ���
				out_time++;/*�Գ��߽��������*/
			else
				out_time=0;
			s=(int)((float)out_time*_P);//ע����ʱ����������
			if(tu_off+s>speed_max)s=speed_max-tu_off;//��������ٶ�
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
			//��߿���
			if((Sen_Data.AD_left<Sen_Data.AD_right))
			{
				Set_motor(tu_off+s,-200,2);
			}
			//�ұ߿���
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


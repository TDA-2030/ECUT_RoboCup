

/* ------------  ͷ�ļ�  -------------- */
#include "..\Uplib\UP_System.h"
#include "..\user\mydef.h"
#include "..\user\lower.h"


u32 Sys_Time=0;      //ϵͳʱ�䣬�ӿ�����ʼ��ʱ����λms
sensor_struct Sen_Data;  //���������ݽṹ��


 //LED�ƿ���
void Set_LED(u8 val)
{
	switch(val)
	{
		case 1:UP_IOout_SetIO(0,1);UP_IOout_SetIO(1,0);break;
		case 2:UP_IOout_SetIO(0,0);UP_IOout_SetIO(1,1);break;
		case 3:UP_IOout_SetIO(0,1);UP_IOout_SetIO(1,1);break;
		default:break;
	}
}
//����ֵ����
int myabs(int a)
{
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

//��ʱ���жϴ�����
void TimerHadler0(u32 u) //��ʱ��⴫��������
{
	Sys_Time++;//ϵͳʱ���Լ�һ����
}


//������ʼ��
void machine_init(void)
{
	//�ȳ�ʼ����ʱ��
	UP_Timer_SetHadler(TIMER_CHANNEL0 , TimerHadler0);  //����жϺ���TimerHadler0
	UP_Timer_EnableIT(TIMER_CHANNEL0 , 1000);  //��λ��΢��,��ʱ1ms
	
	UP_System_Init();//ϵͳ��ʼ��
    UP_CDS_SetMode(1,CDS_SEVMODE);
    UP_CDS_SetMode(2,CDS_SEVMODE);
    UP_CDS_SetMode(3,CDS_SEVMODE);
    UP_CDS_SetMode(4,CDS_SEVMODE);
    UP_CDS_SetMode(5,CDS_SEVMODE);
    UP_CDS_SetMode(6,CDS_SEVMODE);
    UP_CDS_SetMode(7,CDS_SEVMODE);
    UP_CDS_SetMode(8,CDS_SEVMODE);
	UP_CDS_SetMode(9,CDS_SEVMODE);
    UP_CDS_SetMode(10,CDS_SEVMODE);
    UP_CDS_SetMode(MOTO_L_ID,CDS_MOTOMODE);
    UP_CDS_SetMode(MOTO_R_ID,CDS_MOTOMODE);
	UP_LCD_ShowCharacterString(0,1,"**��������ѧ**");
	UP_LCD_ShowCharacterString(0,2,__DATE__);
	UP_LCD_ShowCharacterString(0,3,__TIME__);
	Set_LED(LED_PINK);
}

//Ĭ�϶���������Ĭ�ϵĶ��λ��״̬
//nMS����ʱ������
void  default_act(int nMS)
{
	Set_servo(610,730,412,93,828,580,272,392,
			  400,630,200,200,200,200,630,400,
			  nMS);
}

//һ���˲�����
int filter(int new_value,float a)
{
	static float value;                    //�˲����ֵ
	value=(a*((float)value)) + ((1-a)*((float)new_value));
	return (int)value;
}
//�������ɼ�����
//mode��Ҫ��ȡ�Ĵ���������
//����0����ȡ�ɹ�
int Get_sensor(SEN_TYPE mode)
{
	const char get_times=8;
	int i;
	if(mode==SEN_GRAY)//��ȡ�Ҷȴ�����
	{
		Sen_Data.AD_left  = 0;
		Sen_Data.AD_right = 0;
		Sen_Data.AD_advan = 0;
		Sen_Data.AD_back  = 0;
		Sen_Data.AD_ave   = 0;
		for(i=0;i<get_times;i++)
		{
			Sen_Data.AD_left  +=UP_ADC_GetValue(AD_CH_L);	
			Sen_Data.AD_right +=UP_ADC_GetValue(AD_CH_R);	
			Sen_Data.AD_advan +=UP_ADC_GetValue(AD_CH_A);
			Sen_Data.AD_back  +=UP_ADC_GetValue(AD_CH_B);
		}
		Sen_Data.AD_left  /=get_times;
		Sen_Data.AD_right /=get_times;
		Sen_Data.AD_advan /=get_times;
		Sen_Data.AD_back  /=get_times;
		
		Sen_Data.AD_right +=AD_right_ADJ;
		Sen_Data.AD_advan +=AD_advan_ADJ;
		Sen_Data.AD_back  +=AD_back_ADJ;
		
		Sen_Data.AD_ave= (Sen_Data.AD_left+Sen_Data.AD_right+Sen_Data.AD_advan+Sen_Data.AD_back)/4;
		return 0;
	}
	else if(mode==SEN_DIS)//��ȡ���봫����
	{
		Sen_Data.dis_advan=0;
		Sen_Data.dis_back=0;
		for(i=0;i<get_times;i++)
		{
			Sen_Data.dis_advan +=UP_ADC_GetValue(dis_CH_A);
			//Sen_Data.dis_back  +=UP_ADC_GetValue(dis_CH_B);
		}
		Sen_Data.dis_advan /=get_times;
		Sen_Data.dis_back  /=get_times;
		return 0;
	}
	else if(mode==SEN_IR)//��ȡ���⴫����
	{
		Sen_Data.inf_L  =UP_ADC_GetIO(inf_CH_L);
		Sen_Data.inf_R  =UP_ADC_GetIO(inf_CH_R);
		Sen_Data.inf_A  =UP_ADC_GetIO(inf_CH_A);
		Sen_Data.inf_B  =UP_ADC_GetIO(inf_CH_B);
		Sen_Data.inf_LA =UP_ADC_GetIO(inf_CH_LA);
		Sen_Data.inf_RA =UP_ADC_GetIO(inf_CH_RA);
		return 0;
	}
	else if(mode==SEN_ANGLE)//��ȡ��Ǵ�����
	{
		Sen_Data.AD_X_angle=0;
		Sen_Data.AD_Y_angle=0;
		for(i=0;i<get_times;i++)
		{
			Sen_Data.AD_X_angle+=UP_ADC_GetValue(Angle_X_CH);
			Sen_Data.AD_Y_angle+=UP_ADC_GetValue(Angle_Y_CH);
		}
		Sen_Data.AD_X_angle/=get_times;
		Sen_Data.AD_Y_angle/=get_times;
		return 0;
	
	}
	return 0;
}

//���õ������
//speed1:��ߵ���ٶ�
//speed2:�ұߵ���ٶ�
//nMS����ʱ������
void Set_motor(int speed1,int speed2,int nMS)
{
	UP_CDS_SetSpeed(MOTO_L_ID,-speed1);
	UP_CDS_SetSpeed(MOTO_R_ID,speed2);
	if(nMS<2) nMS=2;		//������Сʱ�䣬��ָֹ�ʧ
	UP_delay_ms(nMS);
}

//����ٶȹ��ɺ���
//speed_start����ʼ�ٶ�
//speed_end��Ŀ���ٶ�
//MOTORID:Ŀ����
//nms����ʱ������
int Speed_up(int speed_start,int speed_end,MOTORID ID,int nms)
{
	float i,j;
	j=(((float)(speed_end-speed_start)*20)/(float)nms);//ÿ20ms��/��һ��
	if(speed_start<speed_end)//+
		{if(j<0)j=-j;}
	else                     //-
		{if(j>0)j=-j;}
	for(i=speed_start;j>0?i<speed_end:i>speed_end;i+=j)
	{
		if(ID==MOT_L)
			UP_CDS_SetSpeed(MOTO_L_ID,(int)(-i));
		else if(ID==MOT_R)
			UP_CDS_SetSpeed(MOTO_R_ID,(int)i);
		else if(ID==MOT_ALL)
		{
			UP_CDS_SetSpeed(MOTO_L_ID,(int)(-i));
			UP_CDS_SetSpeed(MOTO_R_ID,(int)i);
		}
		
		UP_delay_ms(20);
	}
	return (int)i;
}

//���ö������
//��λ�ô��������ο������ж����λ����0Ϊ�����ϴζ���
void Set_servo(int pos1,int pos2,int pos3,int pos4,int pos5,int pos6,int pos7,int pos8,
			   int spe1,int spe2,int spe3,int spe4,int spe5,int spe6,int spe7,int spe8,
			   int time)
{
	if(pos4>0)
	{
		UP_CDS_SetAngle(9,pos4,spe4);
		UP_CDS_SetAngle(10,pos4,spe4);
	}
	if(pos5>0)
	{
		UP_CDS_SetAngle(7,pos5,spe5);
		UP_CDS_SetAngle(8,pos5,spe5);
	}
	
	if(pos6>0)
		UP_CDS_SetAngle(4,pos6,spe6);
	if(pos7>0)
		UP_CDS_SetAngle(5,pos7,spe7);
	if(pos8>0)
		UP_CDS_SetAngle(6,pos8,spe8);
	
	if(pos3>0)
		UP_CDS_SetAngle(1,pos3,spe3);
	if(pos2>0)
		UP_CDS_SetAngle(2,pos2,spe2);
	if(pos1>0)
		UP_CDS_SetAngle(3,pos1,spe1);
	
	if(time<3) time=3;			//������Сʱ�䣬��ָֹ�ʧ
	UP_delay_ms(time);
}

//ǰ��������������
void farward_fall(void)
{

	Set_servo(493,822,900,330,650,59,218,533,
		      500,500,400,200,200,400,500,500,
		      700);//700
	Set_servo(493,503,900,300,680,71,511,527,
		      500,500,400,200,200,400,500,500,
		      600);//600
	Set_servo(496,493,900,299,820,82,510,526,
		      500,500,400,200,200,400,500,500,
		      750);//700
	
	Speed_up(300,500,MOT_ALL,500);
	Speed_up(500,0,MOT_ALL,500);
	
	Set_servo(0,0,0,467,820,0,0,0,
		     200,200,150,200,150,150,200,200,
		     1000);
	Set_servo(496,493,691,341,822,328,509,526,
			  200,250,300,250,200,300,250,200,
			  700);
	default_act(600);
	
}

//����������������
void back_fall()
{
	Set_servo(472,818,331,93,828,688,190,513,
		      500,650,500,300,300,500,650,500,
		      650);
	Set_servo(478,830,85,237,810,900,183,540,//4��924
		      500,500,400,300,300,400,500,500,
		      500);
	Set_servo(485,493,74,310,822,900,493,545,//4��945
		      500,500,400,130,130,400,500,500,
		      600);
	Set_servo(497,508,77,173,648,900,510,509,//4��920
		      500,500,400,150,150,400,500,500,
		      650);
	Speed_up(-200,-500,MOT_ALL,500);
	Speed_up(-500,0,MOT_ALL,500);
	Set_servo(482,485,61,163,450,959,510,509,
		      200,200,150,200,210,150,200,200,
		      900);
	Set_servo(610,730,412,93,828,580,302,352,
			  200,250,400,330,330,400,250,200,
			  500);
		default_act(600);
}

void show_debug_message(void)
{
	UP_LCD_ShowInt(0,0,Sen_Data.AD_Y_angle);
	UP_LCD_ShowInt(0,0,Sen_Data.AD_X_angle);
	UP_LCD_ShowInt(0,1,Sen_Data.AD_ave);
	
}


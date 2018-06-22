
#ifndef MYDEF_H
#define MYDEF_H

/* ------------  �궨��  -------------- */
#define uint unsigned int

/**********���ID����************/
#define MOTO_L_ID   20  //����
#define MOTO_R_ID   21  //�ҵ��

/**********ͨ������************/
//�Ҷȶ˿ں�
#define AD_CH_L  0    //��
#define AD_CH_R  1    //��
#define AD_CH_A  4    //ǰ
#define AD_CH_B  2    //��

//������
#define dis_CH_A 5    //ǰ

//��Ƕ˿ں�
#define Angle_Y_CH  12  //ǰ����
#define Angle_X_CH  13  //���ҷ���

//����˿ں�
#define inf_CH_L    8   //��
#define inf_CH_R    9   //��
#define inf_CH_A    10   //ǰ
#define inf_CH_B    5   //��
#define inf_CH_LA   6  //��ǰ
#define inf_CH_RA   7  //��ǰ
#define inf_CH_ULA  14   //�����ǰ�� ���ڼ��̨�ӱ�Ե
#define inf_CH_URA  15   //�����ǰ�ҷ� ���ڼ��̨�ӱ�Ե

//������궨��
#define INF_L   (UP_ADC_GetIO(inf_CH_L))
#define INF_R   (UP_ADC_GetIO(inf_CH_R))
#define INF_A   (UP_ADC_GetIO(inf_CH_A))
#define INF_B   (UP_ADC_GetIO(inf_CH_B))
#define INF_LA  (UP_ADC_GetIO(inf_CH_LA))
#define INF_RA  (UP_ADC_GetIO(inf_CH_RA))
#define INF_ULA (UP_ADC_GetIO(inf_CH_ULA))
#define INF_URA (UP_ADC_GetIO(inf_CH_URA))

//Ѱ�Ҷ�У׼����AD_leftΪ��׼
#define AD_right_ADJ (60)    //֮ǰ20
#define AD_advan_ADJ (10)
#define AD_back_ADJ  (-100)

//���ؼ���ֵ����
#define AD_GRAY_LINE   (2420) //�Ҷȱ�Եֵ��С����Ϊ�������Ե   ֮ǰΪ2360  8/16 2281
#define AD_GRAY_EDGA   (1350) //��ʾ̨����ڵĵط��ĻҶ�ֵ,���ڶ��ֵ�̨���
#define AD_GRAY_DIP    (2100) //�㵹֮��ĻҶ�ֵ�����ɹ�С
#define MAX_Angle      (3600) //������ֵ��������Ϊ����
#define MIN_Angle      (550)  //�����Сֵ��С����Ϊ����

//���������ݽṹ��,�������д��������ݣ�Get_sensor������ȡ�����ݱ���������
typedef struct 
{
	int AD_left;   /* �Ҷȴ����� */
	int AD_right;
	int AD_advan;
	int AD_back;
	int AD_ave;
	
	int dis_advan;  /* ���봫���� */
	int dis_back;
	
	int AD_X_angle; /* ��Ǵ����� */
	int AD_Y_angle;
	
	bool inf_L;  /* ���⴫���� */
	bool inf_R;
	bool inf_A;
	bool inf_B;
	bool inf_LA;
	bool inf_RA;
	
}sensor_struct;

//���ö�ٱ���
typedef enum 
{
	MOT_R=1,  /* �ұߵ�� */
	MOT_L,    /* ��ߵ�� */
	MOT_ALL   /* ������� */
}MOTORID;

//����������ö�ٱ���
typedef enum 
{
	SEN_GRAY = 1,	/* �Ҷȴ����� */
	SEN_IR,         /* ���⴫���� */
	SEN_DIS,        /* ���봫���� */
	SEN_ANGLE       /* ��Ǵ����� */
	
} SEN_TYPE;

//ϵͳ״̬ö�ٱ���
typedef enum
{
	SYS_NORMAL = 1,	/* ����̬ */
	SYS_DUMP,       /* ����̬ */
	SYS_ATTACK,     /* ����̬ */
	SYS_FOUNDING,   /* Ѱ�Ҷ���̬ */
	SYS_FOUND       /* ��Ѱ�ҵ�����̬ */
	
} SYS_STATE_e;

extern SYS_STATE_e SYS_State;



#endif

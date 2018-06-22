
#ifndef MYDEF_H
#define MYDEF_H

/* ------------  宏定义  -------------- */
#define uint unsigned int

/**********电机ID定义************/
#define MOTO_L_ID   20  //左电机
#define MOTO_R_ID   21  //右电机

/**********通道定义************/
//灰度端口号
#define AD_CH_L  0    //左
#define AD_CH_R  1    //右
#define AD_CH_A  4    //前
#define AD_CH_B  2    //后

//红外测距
#define dis_CH_A 5    //前

//倾角端口号
#define Angle_Y_CH  12  //前后方向
#define Angle_X_CH  13  //左右方向

//红外端口号
#define inf_CH_L    8   //左
#define inf_CH_R    9   //右
#define inf_CH_A    10   //前
#define inf_CH_B    5   //后
#define inf_CH_LA   6  //左前
#define inf_CH_RA   7  //右前
#define inf_CH_ULA  14   //上面的前左方 用于检测台子边缘
#define inf_CH_URA  15   //上面的前右方 用于检测台子边缘

//红外检测宏定义
#define INF_L   (UP_ADC_GetIO(inf_CH_L))
#define INF_R   (UP_ADC_GetIO(inf_CH_R))
#define INF_A   (UP_ADC_GetIO(inf_CH_A))
#define INF_B   (UP_ADC_GetIO(inf_CH_B))
#define INF_LA  (UP_ADC_GetIO(inf_CH_LA))
#define INF_RA  (UP_ADC_GetIO(inf_CH_RA))
#define INF_ULA (UP_ADC_GetIO(inf_CH_ULA))
#define INF_URA (UP_ADC_GetIO(inf_CH_URA))

//寻灰度校准，以AD_left为基准
#define AD_right_ADJ (60)    //之前20
#define AD_advan_ADJ (10)
#define AD_back_ADJ  (-100)

//边沿极限值定义
#define AD_GRAY_LINE   (2420) //灰度边缘值，小于认为到达最边缘   之前为2360  8/16 2281
#define AD_GRAY_EDGA   (1350) //表示台上最黑的地方的灰度值,用于对手掉台检测
#define AD_GRAY_DIP    (2100) //倾倒之后的灰度值，不可过小
#define MAX_Angle      (3600) //倾角最大值，超过认为倒了
#define MIN_Angle      (550)  //倾角最小值，小于认为倒了

//传感器数据结构体,包含所有传感器数据，Get_sensor函数读取的数据保存在这里
typedef struct 
{
	int AD_left;   /* 灰度传感器 */
	int AD_right;
	int AD_advan;
	int AD_back;
	int AD_ave;
	
	int dis_advan;  /* 距离传感器 */
	int dis_back;
	
	int AD_X_angle; /* 倾角传感器 */
	int AD_Y_angle;
	
	bool inf_L;  /* 红外传感器 */
	bool inf_R;
	bool inf_A;
	bool inf_B;
	bool inf_LA;
	bool inf_RA;
	
}sensor_struct;

//马达枚举变量
typedef enum 
{
	MOT_R=1,  /* 右边电机 */
	MOT_L,    /* 左边电机 */
	MOT_ALL   /* 两个电机 */
}MOTORID;

//传感器类型枚举变量
typedef enum 
{
	SEN_GRAY = 1,	/* 灰度传感器 */
	SEN_IR,         /* 红外传感器 */
	SEN_DIS,        /* 距离传感器 */
	SEN_ANGLE       /* 倾角传感器 */
	
} SEN_TYPE;

//系统状态枚举变量
typedef enum
{
	SYS_NORMAL = 1,	/* 正常态 */
	SYS_DUMP,       /* 倒下态 */
	SYS_ATTACK,     /* 攻击态 */
	SYS_FOUNDING,   /* 寻找对手态 */
	SYS_FOUND       /* 已寻找到对手态 */
	
} SYS_STATE_e;

extern SYS_STATE_e SYS_State;



#endif

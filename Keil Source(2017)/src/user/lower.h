/******************************************************************/

/******************************************************************/

#ifndef LOWER_H
#define LOWER_H


extern u32 Sys_Time;
extern sensor_struct Sen_Data;  //传感器数据结构体

//LED灯颜色定义
#define LED_RED   1
#define LED_BLUE  2
#define LED_PINK  3

//函数声明

void Set_LED(u8 val);
int myabs(int a);
void machine_init(void);
void  default_act(int nMS);
int filter(int val,float a);//一阶滤波函数
int Get_sensor(SEN_TYPE mode);
void Set_motor(int speed1,int speed2,int nMS);
int Speed_up(int speed_start,int speed_end,MOTORID ID,int nms);
void Set_servo(int pos1,int pos2,int pos3,int pos4,int pos5,int pos6,int pos7,int pos8,
			   int spe1,int spe2,int spe3,int spe4,int spe5,int spe6,int spe7,int spe8,
			   int time);
void farward_fall(void);
void back_fall(void);


#endif

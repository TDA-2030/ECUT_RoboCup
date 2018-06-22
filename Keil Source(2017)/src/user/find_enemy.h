/******************************************************************/

/******************************************************************/

#ifndef FIND_ENEMY_H
#define FIND_ENEMY_H


//找对手状态枚举变量
typedef enum
{
	ENEMY_NONE = 1,  /* 没有敌人 */
	ENEMY_A,       /* 对手在前方找到 */
	ENEMY_RA,	   /* 对手在右前方找到 */
	ENEMY_LA,      /* 对手在左前方找到 */
	ENEMY_R,	   /* 对手在右方找到 */
	ENEMY_L,       /* 对手在左方找到 */
	
} enemy_state_e;
extern enemy_state_e enemy_sta;

//函数声明
void find_enemy(void);


#endif

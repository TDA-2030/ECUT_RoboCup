/******************************************************************/

/******************************************************************/

#ifndef FIND_ENEMY_H
#define FIND_ENEMY_H


//�Ҷ���״̬ö�ٱ���
typedef enum
{
	ENEMY_NONE = 1,  /* û�е��� */
	ENEMY_A,       /* ������ǰ���ҵ� */
	ENEMY_RA,	   /* ��������ǰ���ҵ� */
	ENEMY_LA,      /* ��������ǰ���ҵ� */
	ENEMY_R,	   /* �������ҷ��ҵ� */
	ENEMY_L,       /* ���������ҵ� */
	
} enemy_state_e;
extern enemy_state_e enemy_sta;

//��������
void find_enemy(void);


#endif

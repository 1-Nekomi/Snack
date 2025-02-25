#ifndef _SNACK_H_
#define _SNACK_H_

#define MAP_MAXSIZE 25
#define CHAR_INTERVAL 5 //�ַ���������������ĸ�̶����������
#define SPEED 500 //ms��λ����ʾ̰���߶�������һ����

typedef int Object;
typedef char Direction;

#define FALSE 0
#define TRUE 1

#define ROAD 0x10
#define HEAD 0x11
#define BODY 0x12
#define FOOD 0x13

#define FRONT "/\\"
#define BACK "\\/"
#define LEFT "< "
#define RIGHT " >"

typedef struct{
	int x[MAP_MAXSIZE*MAP_MAXSIZE];//̰���ߵ�x���꣬ͷ�±�Ϊ0������������������
	int y[MAP_MAXSIZE*MAP_MAXSIZE];//̰���ߵ�x���꣬ͷ�±�Ϊ0������������������
	int length;//̰���ߵĳ���
	char Fr[3];//̰����ǰ������,���ַ���ʾ
}Snake;

//��Ϸ��ں���
void GameStart();

#endif 

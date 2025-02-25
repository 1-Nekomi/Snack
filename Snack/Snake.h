#ifndef _SNACK_H_
#define _SNACK_H_

#define MAP_MAXSIZE 25
#define CHAR_INTERVAL 5 //字符输出间隔，用于字母固定输出函数中
#define SPEED 500 //ms单位，表示贪吃蛇多少秒走一格子

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
	int x[MAP_MAXSIZE*MAP_MAXSIZE];//贪吃蛇的x坐标，头下标为0，身体依次往后延申
	int y[MAP_MAXSIZE*MAP_MAXSIZE];//贪吃蛇的x坐标，头下标为0，身体依次往后延申
	int length;//贪吃蛇的长度
	char Fr[3];//贪吃蛇前进方向,用字符表示
}Snake;

//游戏入口函数
void GameStart();

#endif 

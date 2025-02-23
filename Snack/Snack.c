/*CopyRight(C)2025-TypeDefine  
*/

#include"Snack.h"
#include"OutColor.h"
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<conio.h>
#include<Windows.h>

#define HideCursor printf("%c[?25l",27) //���ع��
#define ShowCursor printf("%c[?25h",27) //��ʾ���
#define gotoxy(x,y) printf("%c[%d;%dH",27,y,x) //�ƶ���굽(x.y)λ��

#define ClearLine(x,y) \
gotoxy(x,y);printf("                                               \
         ");gotoxy(x,y)//���ĳһ��

#define FixedStr(x,y,str) if(!FixStr(x,y,str))goto Reset

int map[MAP_MAXSIZE][MAP_MAXSIZE];
Snack player;

//����̨����ȡXY����
static void GetCursorXY(int* row, int* col);

//��Ч�ַ����
static void StrPrint(const char * str);

//ѡ��xy���꣬��ʼ���ѡ���ַ���
static int FixStr(const int x, const int y, const char * str);

//ʱͣ����,����Ϊ�˼��ݲ�ʹ��usleep��������msΪ��λ
static void Delay(int time);

//������Ӳ�����������0-24�����
static int random();

//��Ҫ���ڷ����ַ��ĸ���
static void StrCpy(char *s1, const char * s2);

//��Ҫ���ڷ����ַ��ıȽ�
static int StrCmp(const char * s1, const char * s2);

//�߳�ʼ��
static void InitSnack(Snack *sna);

//��ͼ��ʼ��
static void InitMap();

//��ͼ�������
static void SetMap(int x,int y,Object obj);

//��ͼ����
static void MapView();

//�߷������
static void CtrlSnack(Snack *sna,Direction *dire);


/*
��������ʵ��
*/
static void GetCursorXY(int* row, int* col) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	*row = csbi.dwCursorPosition.Y + 1; // �кŴ�1��ʼ
	*col = csbi.dwCursorPosition.X + 1; // �кŴ�1��ʼ
}

static void StrPrint(const char * str) {
	int length = strlen(str);
	for (int i = 0; i < length; i++) {
		printf(RED"%c\b"COLOR_END, str[i]); Delay(CHAR_INTERVAL);
		printf(ORANGE"%c\b"COLOR_END, str[i]); Delay(CHAR_INTERVAL);
		printf(L_BLUE"%c\b"COLOR_END, str[i]); Delay(CHAR_INTERVAL);
		printf(WHITE"%c"COLOR_END, str[i]); Delay(CHAR_INTERVAL);
	}
}

static int FixStr(const int x, const int y, const char * str) {
	//��¼ԭ����������
	int rx, ry;
	GetCursorXY(&ry,&rx);
	if (rx == -1 || ry == -1)return FALSE;
	
	ClearLine(x, y);
	gotoxy(x, y);
	StrPrint(str);
	gotoxy(rx, ry);

	return TRUE;
}

static void Delay(int milliseconds) {
	clock_t start = clock();
	// ת��ΪĿ��ʱ��������
	clock_t required_cycles = (milliseconds * CLOCKS_PER_SEC) / 1000;

	while (clock() - start < required_cycles);
}

static int random() {
	static const long long seed[] = {
		101, 103, 107, 109, 113, 127, 131, 137,
		139, 149, 151, 157, 163, 167, 173, 179
	};
	static int index = -1;
	static unsigned int counter = 0;  //��̬������,���ͬһ�����ظ�����

	index++;
	index %= 16;
	counter++;

	time_t T = time(NULL);
	long long mix = (T ^ counter) + seed[(index + 4) % 16];  // ͨ������ƫ�����������

	long long value = mix % seed[index];
	return (int)(value % 25);
}

static void StrCpy(char *s1, const char * s2) {
	s1[0] = s2[0];
	s1[1] = s2[1];
}

static int StrCmp(const char * s1, const char * s2) {
	if (s1[0] == s2[0] && s1[1] == s2[1]) return TRUE;
	return FALSE;
}

static void InitSnack(Snack *sna) {
	StrCpy(sna->Fr,FRONT);
	sna->length = 1;
	sna->x[0] = MAP_MAXSIZE / 2;
	sna->y[0] = MAP_MAXSIZE / 2;
	SetMap(sna->x[0], sna->y[0], HEAD);
}

static void InitMap() {
	for (int i = 0; i < MAP_MAXSIZE; i++) {
		for (int j = 0; j < MAP_MAXSIZE; j++) {
			map[i][j] = ROAD;
		}
	}
}

static void SetMap(int x, int y, Object obj) {
	if (x<0 || x>MAP_MAXSIZE || y<0 || y>MAP_MAXSIZE)return ;

	if (obj == ROAD || obj == HEAD || obj == BODY || obj == FOOD) {
		map[x][y] = obj;
	}
}

static void MapView(){
	for(int i=0;i<MAP_MAXSIZE;i++){
		for(int j=0;j<MAP_MAXSIZE;j++){
			switch(map[i][j]){
				case ROAD:printf(BACK_WHITE"  "COLOR_END);break;
				case HEAD:printf(BACK_YELLOW "%s" COLOR_END,player.Fr); break;
				case BODY:printf(BACK_YELLOW"  "COLOR_END);break;
				case FOOD:printf(BACK_WHITE BROWN"<>"COLOR_END);break;
				default:printf(BACK_RED"!!"COLOR_END);
			}
		}
		printf("\n");
	}
}

static void CtrlSnack(Snack *sna,Direction *dire) {
	if (StrCmp(dire, FRONT) != TRUE && StrCmp(dire, BACK) != TRUE
		&& StrCmp(dire, LEFT) != TRUE && StrCmp(dire, RIGHT) != TRUE)return;
	StrCpy(sna->Fr,dire);
}

void GameStart(){
Reset:
	gotoxy(0, 0);
	InitMap();
	InitSnack(&player);
	HideCursor;
	char order = '\0';
	FixedStr(MAP_MAXSIZE * 2 + 5, 3, "Welcome to the Greedy Snake Game!");
	FixedStr(MAP_MAXSIZE * 2 + 5, 5, "Your score:");
	while(1){
		MapView();
		if (_kbhit()) {
			order = _getch();
			switch (order) {
				case 'W':CtrlSnack(&player, FRONT); break;
				case 'D':CtrlSnack(&player, RIGHT); break;
				case 'A':CtrlSnack(&player, LEFT); break;
				case 'S':CtrlSnack(&player, BACK); break;
				case 'w':CtrlSnack(&player, FRONT); break;
				case 'd':CtrlSnack(&player, RIGHT); break;
				case 'a':CtrlSnack(&player, LEFT); break;
				case 's':CtrlSnack(&player, BACK); break;
			}
		}
		gotoxy(0,0);
	}
} 

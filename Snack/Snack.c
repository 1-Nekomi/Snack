/*CopyRight(C)2025-TypeDefine  
*/

#include"Snack.h"
#include"OutColor.h"
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<conio.h>
#include<Windows.h>

#define HideCursor printf("%c[?25l",27) //隐藏光标
#define ShowCursor printf("%c[?25h",27) //显示光标
#define gotoxy(x,y) printf("%c[%d;%dH",27,y,x) //移动光标到(x.y)位置

#define ClearLine(x,y) \
gotoxy(x,y);printf("                                        \
         ");gotoxy(x,y)//清除某一行

#define FixedStr(x,y,str) if(!FixStr(x,y,str))goto Reset

int map[MAP_MAXSIZE][MAP_MAXSIZE];
Snack player;
char IsGaming = FALSE;
long score = 0;
long Maxscore = 0;

//控制台光标获取XY坐标
static void GetCursorXY(int* row, int* col);

//特效字符输出
static void StrPrint(const char * str);

//选定xy坐标，开始输出选定字符串
static int FixStr(const int x, const int y, const char * str);

//时停函数,这里为了兼容不使用usleep函数，以ms为单位
static void Delay(int time);

//随机种子产生器，产生0-24随机数
static int random();

//主要用于方向字符的更新
static void StrCpy(char *s1, const char * s2);

//主要用于方向字符的比较
static int StrCmp(const char * s1, const char * s2);

//蛇初始化
static void InitSnack(Snack *sna);

//地图初始化
static void InitMap();

//地图物品放置
static void SetMap(int x,int y,Object obj);

//清除地图某一格
static void ClearMap(int x, int y);

//地图绘制
static void MapView();

//蛇方向控制
static void CtrlSnack(Snack *sna,Direction *dire);

//食物随机生成
static void SetFood();

//蛇前进
static int SnackHeadMove(Snack *sna);

//蛇身体前进，额外需要头结点移动前的坐标数据
static void SnackBodyMove(Snack *sna,int x,int y);

//蛇吃豆
static int SnackEat(Snack *sna);


/*
函数具体实现
*/
static void GetCursorXY(int* row, int* col) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	*row = csbi.dwCursorPosition.Y + 1; // 行号从1开始
	*col = csbi.dwCursorPosition.X + 1; // 列号从1开始
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
	//记录原来光标的坐标
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
	// 转换为目标时钟周期数
	clock_t required_cycles = (milliseconds * CLOCKS_PER_SEC) / 1000;

	while (clock() - start < required_cycles);
}

static int random() {
	static const long long seed[] = {
		101, 103, 107, 109, 113, 127, 131, 137,
		139, 149, 151, 157, 163, 167, 173, 179
	};
	static int index = -1;
	static unsigned int counter = 0;  //动态计数器,解决同一秒内重复问题

	index++;
	index %= 16;
	counter++;

	time_t T = time(NULL);
	long long mix = (T ^ counter) + seed[(index + 4) % 16];  // 通过异或和偏移增加随机性

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

	if ((obj == HEAD || obj == BODY || obj == FOOD) && (map[x][y] == ROAD || map[x][y] == FOOD)) {
		map[x][y] = obj;
	}
}

static void ClearMap(int x, int y) {
	if (x<0 || x>MAP_MAXSIZE || y<0 || y>MAP_MAXSIZE)return;

	map[x][y] = ROAD;
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

	//只有头时可以随意调整方向，有身子时不能反方向移动
	if(sna->length == 1)
		StrCpy(sna->Fr,dire);
	else {
		if (StrCmp(dire, FRONT) == TRUE && StrCmp(sna->Fr, BACK) == TRUE) return;
		else if (StrCmp(dire, BACK) == TRUE && StrCmp(sna->Fr, FRONT) == TRUE)return;
		else if (StrCmp(dire, LEFT) == TRUE && StrCmp(sna->Fr, RIGHT) == TRUE)return;
		else if (StrCmp(dire, RIGHT) == TRUE && StrCmp(sna->Fr, LEFT) == TRUE)return;
		StrCpy(sna->Fr, dire);
	}
	SnackHeadMove(sna);
}

static void SetFood() {
	int fx, fy;
	while ((fx = random())>=MAP_MAXSIZE || fx<0);
	while ((fy = random())>=MAP_MAXSIZE || fy<0);
	SetMap(fx,fy,FOOD);
}

static int SnackHeadMove(Snack *sna) {
	//判断蛇的前进方向，然后根据下一格的内容进行判断并移动
	if (StrCmp(sna->Fr, RIGHT)) {
		if (sna->y[0] + 1 >= MAP_MAXSIZE) return FALSE;
		switch (map[sna->x[0]][sna->y[0]+1]) {
			case ROAD: { 
				sna->y[0]++; 
				SetMap(sna->x[0], sna->y[0], HEAD); 
			}break;

			case FOOD: { 
				sna->y[0]++; 
				SetMap(sna->x[0], sna->y[0], HEAD); 
				if (!SnackEat(sna))return FALSE;
			}break;

			default:return FALSE; break;
		}

		if(sna->length == 1)
			ClearMap(sna->x[0], sna->y[0] - 1);
		else {
			ClearMap(sna->x[0], sna->y[0] - 1);
			SetMap(sna->x[0], sna->y[0] - 1, BODY);
		}

		SnackBodyMove(sna, sna->x[0], sna->y[0] - 1);
	}
	else if (StrCmp(sna->Fr, LEFT)) {
		if (sna->y[0] - 1 < 0) return FALSE;
		switch (map[sna->x[0]][sna->y[0] - 1]) {
			case ROAD: { 
				sna->y[0]--; 
				SetMap(sna->x[0], sna->y[0], HEAD); 
			}break;

			case FOOD: {
				sna->y[0]--;
				SetMap(sna->x[0], sna->y[0], HEAD); 
				if (!SnackEat(sna))return FALSE;
			}break;

			default:return FALSE; break;
		}

		if (sna->length == 1)
			ClearMap(sna->x[0], sna->y[0] + 1);
		else {
			ClearMap(sna->x[0], sna->y[0] + 1);
			SetMap(sna->x[0], sna->y[0] + 1, BODY);
		}
			
		SnackBodyMove(sna, sna->x[0], sna->y[0] + 1);
	}
	else if (StrCmp(sna->Fr, BACK)) {
		if (sna->x[0] + 1 >= MAP_MAXSIZE) return FALSE;
		switch (map[sna->x[0] + 1][sna->y[0]]) {
			case ROAD: { 
				sna->x[0]++; 
				SetMap(sna->x[0], sna->y[0], HEAD); 
			}break;

			case FOOD: { 
				sna->x[0]++; 
				SetMap(sna->x[0], sna->y[0], HEAD); 
				if (!SnackEat(sna))return FALSE;
			}break;

			default:return FALSE; break;
		}

		if(sna->length == 1)
			ClearMap(sna->x[0] - 1, sna->y[0]);
		else {
			ClearMap(sna->x[0] - 1, sna->y[0]);
			SetMap(sna->x[0] - 1, sna->y[0], BODY);
		}
		SnackBodyMove(sna, sna->x[0] - 1, sna->y[0]);
	}
	else if (StrCmp(sna->Fr, FRONT)) {
		if (sna->x[0] - 1 < 0) return FALSE;
		switch (map[sna->x[0] - 1][sna->y[0]]) {
			case ROAD: { 
				sna->x[0]--; 
				SetMap(sna->x[0], sna->y[0], HEAD);
			} break;

			case FOOD: { 
				sna->x[0]--; 
				SetMap(sna->x[0], sna->y[0], HEAD); 
				if(!SnackEat(sna))return FALSE; 
			}break;

			default:return FALSE; break;
		}

		if(sna->length == 1)
			ClearMap(sna->x[0] + 1, sna->y[0]);
		else {
			ClearMap(sna->x[0] + 1, sna->y[0]);
			SetMap(sna->x[0] + 1, sna->y[0], BODY);
		}
		SnackBodyMove(sna, sna->x[0] + 1, sna->y[0]);
	}
	return TRUE;
}

static void SnackBodyMove(Snack* sna,int x,int y) {
	if (x<0 || x>MAP_MAXSIZE || y<0 || y>MAP_MAXSIZE )return;

	for (int i = sna->length-1; i >= 1 ; i--) {
		if (i == sna->length - 1) 
			ClearMap(sna->x[i], sna->y[i]);
		else
			SetMap(sna->x[i], sna->y[i], BODY);

		if (i == 1) {
			sna->x[i] = x;
			sna->y[i] = y;
			break;
		}

		sna->x[i] = sna->x[i - 1];
		sna->y[i] = sna->y[i - 1];
		
	}
}

static int SnackEat(Snack* sna) {
	//蛇只有头的时候，下一个方块生成于头前进方向的正后方
	score += 5;
	if (sna->length == 1) {
		if (StrCmp(sna->Fr, FRONT)) {
			if (sna->x[0] + 1 < 0) return FALSE;
			sna->x[1] = sna->x[0] + 1;
			sna->y[1] = sna->y[0];
		}
		else if (StrCmp(sna->Fr, BACK)) {
			if (sna->x[0] - 1 < 0) return FALSE;
			sna->x[1] = sna->x[0] - 1;
			sna->y[1] = sna->y[0];
		}
		else if (StrCmp(sna->Fr, LEFT)) {
			if (sna->y[0] + 1 < 0) return FALSE;
			sna->x[1] = sna->x[0];
			sna->y[1] = sna->y[0] + 1;
		}
		else if (StrCmp(sna->Fr, RIGHT)) {
			if (sna->y[0] - 1 < 0) return FALSE;
			sna->x[1] = sna->x[0];
			sna->y[1] = sna->y[0] - 1;
		}
		SetMap(sna->x[1], sna->y[1], BODY);
		sna->length++;
		return TRUE;
	}

	//蛇有两格及以上，随机生成于身体尾部格四周空闲的区域

	int lx = sna->x[sna->length - 1];
	int ly = sna->y[sna->length - 1];

	//尝试寻找其上下左右可能存在的空位
	if (lx + 1 < MAP_MAXSIZE && map[lx + 1][ly] == ROAD) {
		sna->x[sna->length] = lx + 1;
		sna->y[sna->length] = ly;
		SetMap(lx + 1, ly, BODY);
	}
	else if (lx - 1 >= 0 && map[lx - 1][ly] == ROAD) {
		sna->x[sna->length] = lx - 1;
		sna->y[sna->length] = ly;
		SetMap(lx - 1, ly, BODY);
	}
	else if (ly + 1 < MAP_MAXSIZE && map[lx][ly + 1] == ROAD) {
		sna->x[sna->length] = lx;
		sna->y[sna->length] = ly + 1;
		SetMap(lx, ly + 1, BODY);
	}
	else if (ly - 1 >= 0 && map[lx][ly - 1] == ROAD) {
		sna->x[sna->length] = lx;
		sna->y[sna->length] = ly - 1;
		SetMap(lx, ly - 1, BODY);
	}

	sna->length++;
	return TRUE;
}

void GameStart(){
	HideCursor;
	FixedStr(MAP_MAXSIZE * 2 + 5, 3, "Welcome to the Greedy Snake Game!");
	FixedStr(MAP_MAXSIZE * 2 + 5, 5, "Enter WASD to control the direction of the snake");
	FixedStr(MAP_MAXSIZE * 2 + 5, 7, "Your best score:");
	FixedStr(MAP_MAXSIZE * 2 + 5, MAP_MAXSIZE/2, "Your score:");
Reset:
	gotoxy(0, 0);
	InitMap();
	InitSnack(&player);
	IsGaming = FALSE;
	char order = '\0';
	
	int count = 0;
	score = 0;
	ClearLine(MAP_MAXSIZE * 2 + 16, MAP_MAXSIZE / 2);
	ClearLine(MAP_MAXSIZE * 2 + 21, 7);

	//最好成绩分数输出
	int ly, lx;
	gotoxy(MAP_MAXSIZE * 2 + 21, 7);
	printf(L_RED"%ld"COLOR_END, Maxscore);
	gotoxy(0, 0);

	while(1){
		MapView();
		//读取缓冲区，设置蛇的方向
		if (_kbhit()) {
			IsGaming = TRUE;
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
		if (IsGaming) {
			//count类似于滴答数，用于计数，也相当于另一种程度上的计时
			count++;
			count %= 65536;
			
			//分数面板输出
			GetCursorXY(&ly, &lx);
			gotoxy(MAP_MAXSIZE * 2 + 16, MAP_MAXSIZE / 2);
			printf(L_BLUE"%ld"COLOR_END, score);
			gotoxy(lx, ly);


			//该部分用于设定放置Food和蛇行走的间隔，无法设置精确时间
			if (count % 256 == 0)
				SetFood();
			if (count % 16 == 0) {
				if (!SnackHeadMove(&player)) {
					FixedStr(MAP_MAXSIZE * 2 + 5, 15, "Game Over");
					_getch();
					ClearLine(MAP_MAXSIZE * 2 + 5, 15);
					if (score > Maxscore) Maxscore = score;
					goto Reset;
				}
			}
			if (count % 128 == 0) score += 10;
		}
		gotoxy(0,0);
	}
} 

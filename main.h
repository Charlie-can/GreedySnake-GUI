#pragma once
#pragma warning(disable:4996)

#include <easyx.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <process.h>


//创建地图宽度
#define WIDTH 50
//创建地图高度
#define HEIGHT 40
//初始游戏速度
#define SPEED 4
//单个方块大小
#define BLOCKSIZE 20
//虚拟按键映射函数
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
//虚拟按键映射：左
#define LEFT 37
//虚拟按键映射：上
#define UP 38
//虚拟按键映射：右
#define RIGHT 39
//虚拟按键映射：下
#define DOWN 40
//生成游戏地图大小初始化
char map[HEIGHT][WIDTH] = { 0 };
//食物生成标志
int fed = 0;
//已吃食物数量
int fedNum = -1;
// 游戏速度变量
int speed = SPEED;
//控制多线程是否继续标志
bool bSnakeP1Continue = 0, bSnakeP2Continue = 0;
//蛇P1长度
int lengthP1 = 3;
//蛇P2长度
int lengthP2 = 3;
//是否开启对战模式
bool isCombat = 0;
//P1将要添加身体数量
int additionP1 = 0;
//P2将要添加身体数量
int additionP2 = 0;

//时间变量
time_t timep;
//时间戳
time_t sratTimeStamp;
//互斥锁
HANDLE mutex = NULL;
//P1句柄，P2句柄
HANDLE gameHandleP1 = NULL, gameHandleP2 = NULL;

//蛇身结构体
typedef struct snakeStruct
{
	//X轴位置
	int x;
	//Y轴位置
	int y;
	//下一个身体链表
	struct snakeStruct* next;

}snake;


//设置结构体
typedef struct {
	//前十
	int tonTen[10];   
	//是否难度递增
	bool difficultyIncreasing;  
	//是否显示背景竖线
	bool showBackGround;	
	//速度
	int speed;			
	//地图宽度
	int width;			
	//地图高度
	int height;				
	//方块大小
	int blockSize;				

}Setting;

//画蛇结构体
typedef struct {
	//蛇身体颜色
	COLORREF color;
	//蛇的身体结构体指针
	snake* snakePoints;
}drawSnake;

//P1蛇头全局变量
snake* snakeHead;
//P2蛇头全局变量
snake* snakeHeadP2;
//设置结构体
Setting settings;


/// <summary>
/// 保存贪吃蛇设定
/// </summary>
/// <param name="settings">设定结构体指针</param>
void saveSettingsBinary(const Setting* settings);

/// <summary>
/// 读取贪吃蛇设定
/// </summary>
/// <param name="settings">设定结构体指针</param>
void loadSettingsBinary(Setting* settings);


/// <summary>
/// 初始化地图内元素
/// 画出地图框线
/// </summary>
void initMap();

/// <summary>
/// 初始化窗口
/// 生成合适大小
/// </summary>
void initWindow();

/// <summary>
/// 画出地图横线竖线
/// </summary>
void initLine();

/// <summary>
/// 初始化地图
/// 画出蛇在地图的位置
/// </summary>
/// <param name="initP1">是否生成P1</param>
/// <param name="initP2">是否生成P2</param>
void initSnake(bool initP1,bool initP2);

/// <summary>
/// 更新地图数组内的蛇身位置
/// </summary>
/// <param name="snakePoint">对应蛇的蛇头链表</param>
/// <param name="isDelete">
/// 是否删除
/// 当isDelete=1 删除地图数组内对应位置的蛇
/// 当isDelete=0 画出地图数组内对应位置的蛇
/// </param>
void updateMap(snake* snakePoint, int isDelete);

/// <summary>
/// 隐藏蛇身
/// </summary>
/// <param name="snakePoint">对应蛇的蛇头链表</param>
/// <param name="bodyColor">将要替换蛇身的颜色</param>
void hideSnake(snake* snakePoint, COLORREF bodyColor);

/// <summary>
/// 更新蛇
/// 删除最后一个多余蛇尾
/// </summary>
/// <param name="snakePoint">对应蛇的蛇头链表</param>
void updateSnake(snake* snakePoint);

/// <summary>
/// 显示蛇头的位置
/// </summary>
/// <param name="snakePoint">对应蛇的蛇头链表</param>
/// <param name="headColor">将要画出蛇头的颜色</param>
void showSnake(snake* snakePoint, COLORREF headColor);

/// <summary>
/// 释放蛇的链表
/// </summary>
/// <param name="snakeHead">对应蛇的蛇头链表</param>
void freeSnake(snake* snakeHead);

/// <summary>
/// 游戏结束
/// </summary>
/// <param name="x">游戏人数</param>
void gameOver(int x);

/// <summary>
/// 是否死亡
/// 判定蛇是否撞墙或互撞
/// </summary>
/// <param name="snakeP1">判定蛇</param>
/// <param name="snakeP2">被判定蛇 单人时传NULL</param>
/// <returns></returns>
int isDeath(snake* snakeP1, snake* snakeP2);

/// <summary>
/// 生成食物
/// </summary>
void food();


/// <summary>
/// 游戏侧边栏
/// </summary>
/// <param name="start">
/// 是否开始
/// 0 未开始
/// 1 已开始
/// </param>
/// <param name="multi">
/// 是否多人
/// 1 多人
/// 0 单人
/// </param>
void gameBar(bool start, bool multi);


/// <summary>
/// 设置菜单
/// </summary>
void settingMenu();


/// <summary>
/// 清除蛇在画布上的身体
/// </summary>
/// <param name="snake">被清除蛇</param>
void clearSnake(snake* snake);

/// <summary>
/// 清除蛇在画布上的身体
/// </summary>
/// <param name="snake">被清除蛇</param>
void clearSnake(snake* snake);


void saveSettingsBinary(const Setting* settings) {
	FILE* file = fopen("settings.bin", "wb");
	if (file != NULL) {
		fwrite(settings, sizeof(Setting), 1, file);
		fclose(file);
	}
}


void loadSettingsBinary(Setting* settings) {
	FILE* file = fopen("settings.bin", "rb");
	if (file != NULL) {
		fread(settings, sizeof(Setting), 1, file);
		fclose(file);
	}
}

void initMap() {

	//初始化地图为' '
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			map[i][j] = ' ';
		}
	}
	
	//画出地图边界
	for (int i = 0; i < WIDTH; i++)
	{
		map[0][i] = '#';
		map[HEIGHT - 1][i] = '#';
	}


	for (int i = 0; i < HEIGHT; i++)
	{
		map[i][0] = '#';
		map[i][WIDTH - 1] = '#';
	}
}



void initWindow() {
	//初始化窗口大小
	int x = BLOCKSIZE * WIDTH;
	int y = BLOCKSIZE * HEIGHT;

	initgraph(x + 150, y);

	//  设置背景色
	setbkcolor(0xCAE1A4);

	//  使用背景色清空窗体
	cleardevice();

}

void initLine() {

	int x = BLOCKSIZE * WIDTH;
	int y = BLOCKSIZE * HEIGHT;

	//设置线条颜色
	setlinecolor(0xCAE1A4);
	//setlinecolor(0x000000);

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
		{
			if (map[i][j] == '#') {
				setfillcolor(RGB(183, 213, 239));
				solidrectangle(j * BLOCKSIZE, i * BLOCKSIZE, (j + 1) * BLOCKSIZE, (i + 1) * BLOCKSIZE);
			}
		}
	}

	for (int i = BLOCKSIZE; i < x + 1; i += BLOCKSIZE)
	{
		line(i, 0, i, y);


	}
	//X轴竖线



	for (int i = BLOCKSIZE; i < y; i += BLOCKSIZE)
	{
		line(0, i, x, i);

	}
	//Y轴横线

	return;
}

void initSnake(bool initP1, bool initP2) {
	snake* snakeBody;

	//初始化蛇P2
	if (initP2) {
		snakeHeadP2 = (snake*)malloc(sizeof(snake));
		snakeHeadP2->x = WIDTH - 10;
		snakeHeadP2->y = HEIGHT - 5;
		snakeHeadP2->next = NULL;

		snakeBody = (snake*)malloc(sizeof(snake));
		snakeHeadP2->next = snakeBody;
		snakeBody->x = snakeHeadP2->x - 1;
		snakeBody->y = snakeHeadP2->y;
		snakeBody->next = NULL;

		snakeBody = (snake*)malloc(sizeof(snake));
		snakeHeadP2->next->next = snakeBody;
		snakeBody->x = snakeHeadP2->next->x - 1;
		snakeBody->y = snakeHeadP2->next->y;
		snakeBody->next = NULL;
		for (snake* snakePoint = snakeHeadP2; snakePoint; snakePoint = snakePoint->next)
		{
			if (snakePoint == snakeHeadP2) {

				setfillcolor(0x000000);

			}
			else {
				setfillcolor(0xFC5633);
			}
			solidrectangle(((snakePoint->x * BLOCKSIZE) + 1), ((snakePoint->y * BLOCKSIZE) + 1), (((snakePoint->x + 1) * BLOCKSIZE) - 1), (((snakePoint->y + 1) * BLOCKSIZE) - 1));

		}
	}

	//初始化蛇P1
	if (initP1) {
		snakeHead = (snake*)malloc(sizeof(snake));
		snakeHead->x = 5;
		snakeHead->y = 5;
		snakeHead->next = NULL;

		snakeBody = (snake*)malloc(sizeof(snake));
		snakeHead->next = snakeBody;
		snakeBody->x = snakeHead->x - 1;
		snakeBody->y = snakeHead->y;
		snakeBody->next = NULL;

		snakeBody = (snake*)malloc(sizeof(snake));
		snakeHead->next->next = snakeBody;
		snakeBody->x = snakeHead->next->x - 1;
		snakeBody->y = snakeHead->next->y;
		snakeBody->next = NULL;

		for (snake* snakePoint = snakeHead; snakePoint; snakePoint = snakePoint->next)
		{
			if (snakePoint == snakeHead) {

				setfillcolor(RED);

			}
			else {
				setfillcolor(0xFFFFFF);


			}
			solidrectangle(((snakePoint->x * BLOCKSIZE) + 1), ((snakePoint->y * BLOCKSIZE) + 1), (((snakePoint->x + 1) * BLOCKSIZE) - 1), (((snakePoint->y + 1) * BLOCKSIZE) - 1));

		}

	}

	return;
}


void updateMap(snake* snakePoint, int isDelete) {
	//如果是删除
	if (isDelete) {

		for (snakePoint; snakePoint; snakePoint = snakePoint->next)
		{
			map[snakePoint->y][snakePoint->x] = '#';
		}

	}
	else {

		for (snakePoint; snakePoint; snakePoint = snakePoint->next)
		{
			map[snakePoint->y][snakePoint->x] = ' ';
		}

	}
	return;
}

void hideSnake(snake* snakePoint, COLORREF bodyColor) {
	//隐藏头和尾

	for (snakePoint; snakePoint->next; snakePoint = snakePoint->next)
	{
		if (snakePoint) {

			setfillcolor(bodyColor);
			solidrectangle(((snakePoint->x * BLOCKSIZE) + 1), ((snakePoint->y * BLOCKSIZE) + 1), (((snakePoint->x + 1) * BLOCKSIZE) - 1), (((snakePoint->y + 1) * BLOCKSIZE) - 1));

		}
	}
	setfillcolor(0xCAE1A4);
	solidrectangle(((snakePoint->x * BLOCKSIZE) + 1), ((snakePoint->y * BLOCKSIZE) + 1), (((snakePoint->x + 1) * BLOCKSIZE) - 1), (((snakePoint->y + 1) * BLOCKSIZE) - 1));
	return;
}


void updateSnake(snake* snakePoint) {
	for (snakePoint; snakePoint->next->next; snakePoint = snakePoint->next);
	free(snakePoint->next);
	snakePoint->next = NULL;
}

void showSnake(snake* snakePoint, COLORREF headColor) {
	//显示头
	snake* head = snakePoint;
	for (snakePoint; snakePoint->next; snakePoint = snakePoint->next)
	{
		if (snakePoint == head) {
			setfillcolor(headColor);
			solidrectangle(((snakePoint->x * BLOCKSIZE) + 1), ((snakePoint->y * BLOCKSIZE) + 1), (((snakePoint->x + 1) * BLOCKSIZE) - 1), (((snakePoint->y + 1) * BLOCKSIZE) - 1));
		}
	}
	return;
}

void freeSnake(snake* snakeHead) {
	while (snakeHead != NULL) {
		snake* snakePoint = snakeHead;
		snakeHead = snakeHead->next;
		free(snakePoint);
	}
}

int isDeath(snake* snakeP1, snake* snakeP2) {
	Sleep(1);
	if (isCombat) {
		if ((snakeP1->x == snakeP2->x) && (snakeP1->y == snakeP2->y)) {
			bSnakeP1Continue = 0;
			bSnakeP2Continue = 0;
			gameOver(2);
			return 3;
		}
	}
	//双人争霸判断头相撞
	snake* snakePoint;
	for (snakePoint = snakeP1->next; snakePoint; snakePoint = snakePoint->next)
	{
		if (((snakeP1->x == snakePoint->x) && (snakeP1->y == snakePoint->y)) || (snakeP1->x == 0 || snakeP1->x == WIDTH - 1 || snakeP1->y == 0 || snakeP1->y == HEIGHT - 1)) {

			if (snakeP2) {
				bSnakeP1Continue = 0;
				bSnakeP2Continue = 0;

				gameOver(2);
			}
			else
				gameOver(1);
			//如果p2为假，证明单人游戏，否者是双人游戏

			return 1;
		}

	}

	if (snakeP2) {
		for (snakePoint = snakeP2; snakePoint; snakePoint = snakePoint->next)
		{
			if ((snakeP1->x == snakePoint->x) && (snakeP1->y == snakePoint->y)) {

				if(isCombat){
				return 2;
				//争霸被吃
				}
				else {
					bSnakeP1Continue = 0;
					bSnakeP2Continue = 0;
					gameOver(2);
					return 1;

				}
			}
			

		}
	}


	return 0;

}


void food() {

	//如果生成的食物被吃
	if (!fed) {
		int x, y;

		x = rand() % (WIDTH - 2);
		y = rand() % (HEIGHT - 2);

		//判断生成范围
		if (x <= 1 || x >= WIDTH) {
			food();
			return;
		}

		if (y <= 1 || y >= HEIGHT) {
			food();
			return;
		}

		if (map[x][y] != ' ') {
			food();
			return;
		}
		//多线程抢占，上锁
		WaitForSingleObject(mutex, INFINITE);


		map[x][y] = '*';
		setfillcolor(YELLOW);
		solidrectangle(((y * BLOCKSIZE) + 1), ((x * BLOCKSIZE) + 1), (((y + 1) * BLOCKSIZE) - 1), (((x + 1) * BLOCKSIZE) - 1));

		fed = 1;
		fedNum++;

		//解锁
		ReleaseMutex(mutex);

	}
	return;
	//在双人游戏下 ，不会随机生成
}


void gameBar(bool start, bool multi) {

	//初始化字体
	settextcolor(0xffffff);

	TCHAR str[BLOCKSIZE];
	LOGFONT fontSetting;
	gettextstyle(&fontSetting);						// 获取当前字体设置
	fontSetting.lfHeight = BLOCKSIZE+ BLOCKSIZE*0.2;						// 设置字体高度
	_tcscpy(fontSetting.lfFaceName, _T("微软雅黑"));		// 设置字体为
	fontSetting.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&fontSetting);						// 设置字体样式


	time_t nowTime = time(&timep);
	time_t score = (nowTime - sratTimeStamp) + (fedNum * 10);

	//游戏速度递增
	if (speed < 6)
		speed = (score / 100) + SPEED;


	//如果是多人游戏
	if (multi) {
		_stprintf(str, _T("Game Time:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT, str);

		_stprintf(str, _T("%d"), (nowTime - sratTimeStamp));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE*2, str);

		_stprintf(str, _T("Speed:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 4, str);

		_stprintf(str, _T("%d"), speed);
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 6, str);
		
		_stprintf(str, _T("P1 Length:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 8, str);

		_stprintf(str, _T("%d"), lengthP1);
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 10, str);

		_stprintf(str, _T("P2 Length:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 12, str);

		_stprintf(str, _T("%d"), lengthP2);
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 14, str);



		return;
	}
	//是否是开始游戏时
	if (start) {
		_stprintf(str, _T("按下任意键开始"));
		outtextxy(WIDTH * BLOCKSIZE + 5, HEIGHT * BLOCKSIZE / 2, str);

		getch();

		setfillcolor(0xCAE1A4);
		solidrectangle(WIDTH * BLOCKSIZE + 1, HEIGHT * BLOCKSIZE / 2, WIDTH * BLOCKSIZE + 150, HEIGHT * BLOCKSIZE);
	}
	else {

		_stprintf(str, _T("Game Score:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT, str);


		_stprintf(str, _T("%d"), score);
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE*2, str);

		_stprintf(str, _T("Body Length:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 4, str);


		_stprintf(str, _T("%d"), fedNum + 3);
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 6, str);

		_stprintf(str, _T("Speed:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 8, str);

		_stprintf(str, _T("%d"), speed);
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 10, str);

	}
	return;
}




void clearSnake(snake* snakes) {

	//清除蛇在地图上的位置

	snake* snakePoint;
	for (snakePoint = snakes; snakePoint; snakePoint = snakePoint->next)
	{
		if (snakePoint != snakes) {
			setfillcolor(RGB(164, 225, 202));
			map[snakePoint->y][snakePoint->x] = ' ';

		}
		solidrectangle(((snakePoint->x * BLOCKSIZE) + 1), ((snakePoint->y * BLOCKSIZE) + 1), (((snakePoint->x + 1) * BLOCKSIZE) - 1), (((snakePoint->y + 1) * BLOCKSIZE) - 1));
		Sleep(100);
	}

}

void settingMenu() {
	//设置菜单
	cleardevice();

	ExMessage input;
	input = getmessage(EX_MOUSE | EX_KEY);

	LOGFONT fontSetting;
	gettextstyle(&fontSetting);
	TCHAR str[BLOCKSIZE];
	fontSetting.lfHeight = 48;
	settextstyle(&fontSetting);


	loadSettingsBinary(&settings);
	//读取设置



	_stprintf(str, _T("设置"));
	outtextxy(((WIDTH * BLOCKSIZE) / 2) + BLOCKSIZE, (BLOCKSIZE)+BLOCKSIZE * 0.2, str);

	fontSetting.lfHeight = 27;
	settextstyle(&fontSetting);

	_stprintf(str, _T("按下对应字母选择按钮"));
	outtextxy(WIDTH * BLOCKSIZE - BLOCKSIZE * 3, BLOCKSIZE * 2, str);


	fontSetting.lfHeight = 48;
	settextstyle(&fontSetting);

	_stprintf(str, _T("按钮"));
	outtextxy(BLOCKSIZE * 2, BLOCKSIZE * 4, str);


	while (1)
	{
		input = getmessage(EX_MOUSE | EX_KEY);
		// 获取按键信息
		switch (input.message)
		{

		case WM_KEYDOWN:
			if (input.vkcode == VK_ESCAPE)
				return;				// 按 ESC 键退出程序
		}


	}
	saveSettingsBinary(&settings);
	//保存文件

	return;
}
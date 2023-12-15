#pragma once
#pragma warning(disable:4996)

#include <easyx.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <process.h>


#define WIDTH 40
#define HEIGHT 30
#define SPEED 4
#define BLOCKSIZE 25

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

#define LEFT 37
#define UP 38
#define RIGHT 39
#define DOWN 40

char map[HEIGHT][WIDTH] = { 0 };
int fed = 0;
int fedNum = -1;
int speed = SPEED;
bool bSnakeP1Continue = 0, bSnakeP2Continue = 0;

time_t timep;
time_t sratTimeStamp;

HANDLE mutex = NULL;
HANDLE gameHandleP1 = NULL, gameHandleP2 = NULL;


typedef struct snakeStruct
{
	int x;
	int y;
	struct snakeStruct* next;

}snake;

typedef struct {
	int tonTen[10];   //前十
	bool difficultyIncreasing;  //是否难度递增
	bool showBackGround;		//是否显示背景竖线
	int speed;					//速度
	int width;					//地图宽度
	int height;					//地图高度
	int blockSize;				//方块大小

}Setting;

typedef struct {
	COLORREF color;
	snake* snakePoints;
}drawSnake;


snake* snakeHead;
snake* snakeHeadP2;
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
/// <param name="x">
/// 当x等于2时画两条
/// x等于1时画一条
/// 
/// </param>
void initSnake(int x);

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

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			map[i][j] = ' ';
		}
	}
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

void initSnake(int x) {
	snake* snakeBody;

	switch (x)
	{
	case 2:
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

	case 1:
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
			Sleep(100);
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

				bSnakeP1Continue = 0;
				bSnakeP2Continue = 0;

				gameOver(2);
				return 1;
			}

		}
	}
	return 0;

}

void food() {

	if (!fed) {
		int x, y;

		x = rand() % (WIDTH - 2);
		y = rand() % (HEIGHT - 2);

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

		map[x][y] = '*';
		setfillcolor(YELLOW);
		solidrectangle(((y * BLOCKSIZE) + 1), ((x * BLOCKSIZE) + 1), (((y + 1) * BLOCKSIZE) - 1), (((x + 1) * BLOCKSIZE) - 1));

		fed = 1;
		fedNum++;
	}
	return;
	//在双人游戏下 ，不会随机生成
}

void gameBar(bool start, bool multi) {

	settextcolor(0xffffff);

	TCHAR str[BLOCKSIZE];
	LOGFONT fontSetting;
	gettextstyle(&fontSetting);						// 获取当前字体设置
	fontSetting.lfHeight = 27;						// 设置字体高度
	_tcscpy(fontSetting.lfFaceName, _T("微软雅黑"));		// 设置字体为
	fontSetting.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&fontSetting);						// 设置字体样式


	time_t nowTime = time(&timep);
	time_t score = (nowTime - sratTimeStamp) + (fedNum * 10);

	if (speed < 6)
		speed = (score / 100) + SPEED;


	if (multi) {
		_stprintf(str, _T("Game Time:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT, str);

		_stprintf(str, _T("%d"), (nowTime - sratTimeStamp));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE, str);

		_stprintf(str, _T("Speed:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 2, str);

		_stprintf(str, _T("%d"), speed);
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 3, str);

		return;
	}

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
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE, str);

		_stprintf(str, _T("Body Length:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 2, str);


		_stprintf(str, _T("%d"), fedNum + 3);
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 3, str);

		_stprintf(str, _T("Speed:"));
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 4, str);

		_stprintf(str, _T("%d"), speed);
		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 5, str);

	}
	return;
}

void settingMenu() {

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
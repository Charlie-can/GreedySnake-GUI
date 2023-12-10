#pragma once
#pragma warning(disable:4996)

#include <easyx.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <process.h>

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

#define LEFT 37
#define UP 38
#define RIGHT 39
#define DOWN 40


typedef struct snakeStruct
{
	int x;
	int y;
	struct snakeStruct* next;

}snake;

typedef struct {
	int topTen[10];				//前十
	bool difficultyIncreasing;  //是否难度递增
	bool showBackGround;		//是否显示背景竖线
	int speed;					//速度
	int width;					//地图宽度
	int height;					//地图高度
	int blockSize;				//方块大小

}Setting;

snake* snakeHead;
snake* snakeHeadP2;
Setting settings;

int fed = 0;
int fedNum = -1;
bool bSnakeP1Continue = 0, bSnakeP2Continue = 0;

int** map;

time_t timep;
time_t sratTimeStamp;

HANDLE mutex = NULL;
HANDLE gameHandleP1 = NULL, gameHandleP2 = NULL;

//动态创建地图  行 列
void newMap(int rows, int cols);

//初始化地图
void initMap();			

//初始化窗口
void initWindow();	

//初始线条
void initLine();	

//当x=1 生成一条蛇，x=2 生成两条蛇
void initSnake(int x);

//更新蛇在map数组里的位置，传入 蛇 和 是否是蛇头
void updateMap(snake* snakePoint, int isSnakeHead);		

//隐藏蛇头 将蛇头的颜色改为蛇身体的颜色，传入蛇头和蛇身颜色
void hideSnake(snake* snakePoint, COLORREF bodyColor);

//更新蛇 传入蛇
void updateSnake(snake* snakePoint);

//显示蛇头 传入蛇 和 头颜色
void showSnake(snake* snakePoint, COLORREF headColor);

// 释放蛇的链表 传入蛇
void freeSnake(snake* snakeHead);

//游戏结束 传入是几个人游戏
void gameOver(int i);

//判定是否死亡，传入哪一条蛇判定死亡 和被判定蛇
int isDeath(snake* snakeP1, snake* snakeP2);

//生成食物
void food();

//游戏侧边栏 传入 是否开始 是否多人游戏
void gameBar(bool start, bool multi);

// 开始游戏菜单
void settingMenu();

//保存游戏设置
void saveSettingsBinary(const Setting* settings);

//读取游戏设置
void loadSettingsBinary(Setting* settings);




void newMap(int rows, int cols) {

	map = (int**)malloc(rows * sizeof(int*));
	if (map == NULL) {
		return;
	}
	// 为每一行分配内存

	for (int i = 0; i < rows; i++) {
		map[i] = (int*)malloc(cols * sizeof(int));
		if (map[i] == NULL) {
			return;
		}
	}
	//每一行中每一列分配内存

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
		{
			map[i][j] = 0;
		}

	}
	//初始化为0
}

void initMap() {

	for (int i = 0; i < settings.height; i++) {
		for (int j = 0; j < settings.width; j++) {
			map[i][j] = ' ';
		}
	}
	for (int i = 0; i < settings.width; i++)
	{
		map[0][i] = '#';
		map[settings.height - 1][i] = '#';
	}

	for (int i = 0; i < settings.height; i++)
	{
		map[i][0] = '#';
		map[i][settings.width - 1] = '#';
	}
}

void initWindow() {
	int x = settings.blockSize * settings.width;
	int y = settings.blockSize * settings.height;
	initgraph(x + 150, y);

	//  设置背景色
	setbkcolor(0xCAE1A4);

	//  使用背景色清空窗体
	cleardevice();
}

void initLine() {
	int x = settings.blockSize * settings.width;
	int y = settings.blockSize * settings.height;

	if (settings.showBackGround) {
		setlinecolor(0x000000);
	}
	else
	{
		setlinecolor(0xCAE1A4);
	}

	for (int i = 0; i < settings.height; i++) {
		for (int j = 0; j < settings.width; j++)
		{
			if (map[i][j] == '#') {
				setfillcolor(RGB(183, 213, 239));
				solidrectangle(j * settings.blockSize, i * settings.blockSize, (j + 1) * settings.blockSize, (i + 1) * settings.blockSize);
			}
		}
	}
	for (int i = settings.blockSize; i < x + 1; i += settings.blockSize)
	{
		line(i, 0, i, y);


	}
	//X轴竖线



	for (int i = settings.blockSize; i < y; i += settings.blockSize)
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
		snakeHeadP2->x = settings.width - 10;
		snakeHeadP2->y = settings.height - 5;
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
			solidrectangle(((snakePoint->x * settings.blockSize) + 1), ((snakePoint->y * settings.blockSize) + 1), (((snakePoint->x + 1) * settings.blockSize) - 1), (((snakePoint->y + 1) * settings.blockSize) - 1));

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
			solidrectangle(((snakePoint->x * settings.blockSize) + 1), ((snakePoint->y * settings.blockSize) + 1), (((snakePoint->x + 1) * settings.blockSize) - 1), (((snakePoint->y + 1) * settings.blockSize) - 1));

		}

	}

	return;
}

void updateMap(snake* snakePoint, int isSnakeHead) {

	if (isSnakeHead) {

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
			solidrectangle(((snakePoint->x * settings.blockSize) + 1), ((snakePoint->y * settings.blockSize) + 1), (((snakePoint->x + 1) * settings.blockSize) - 1), (((snakePoint->y + 1) * settings.blockSize) - 1));

		}
	}
	setfillcolor(0xCAE1A4);
	solidrectangle(((snakePoint->x * settings.blockSize) + 1), ((snakePoint->y * settings.blockSize) + 1), (((snakePoint->x + 1) * settings.blockSize) - 1), (((snakePoint->y + 1) * settings.blockSize) - 1));

	return;
}

void updateSnake(snake* snakePoint) {
	for (snakePoint; snakePoint->next->next; snakePoint = snakePoint->next);
	free(snakePoint->next);
	snakePoint->next = NULL;
}

void showSnake(snake* snakePoint, COLORREF headColor) {
	//显示头和身1
	snake* head = snakePoint;
	for (snakePoint; snakePoint->next; snakePoint = snakePoint->next)
	{
		if (snakePoint == head) {
			setfillcolor(headColor);
			solidrectangle(((snakePoint->x * settings.blockSize) + 1), ((snakePoint->y * settings.blockSize) + 1), (((snakePoint->x + 1) * settings.blockSize) - 1), (((snakePoint->y + 1) * settings.blockSize) - 1));
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
		if (((snakeP1->x == snakePoint->x) && (snakeP1->y == snakePoint->y)) || (snakeP1->x == 0 || snakeP1->x == settings.width - 1 || snakeP1->y == 0 || snakeP1->y == settings.height - 1)) {

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

		x = rand() % (settings.width - 2);
		y = rand() % (settings.height - 2);

		if (x <= 1 || x >= settings.width) {
			food();
			return;
		}

		if (y <= 1 || y >= settings.height) {
			food();
			return;
		}

		if (map[x][y] != ' ') {
			food();
			return;
		}

		map[x][y] = '*';
		setfillcolor(YELLOW);
		solidrectangle(((y * settings.blockSize) + 1), ((x * settings.blockSize) + 1), (((y + 1) * settings.blockSize) - 1), (((x + 1) * settings.blockSize) - 1));

		fed = 1;
		fedNum++;
	}
	return;
	//在双人游戏下 ，不会随机生成
}

void gameBar(bool start, bool multi) {

	settextcolor(0xffffff);
	 
	TCHAR str[25] = { 0 };
	LOGFONT fontSetting;
	gettextstyle(&fontSetting);						// 获取当前字体设置
	fontSetting.lfHeight = 27;						// 设置字体高度
	_tcscpy(fontSetting.lfFaceName, _T("微软雅黑"));		// 设置字体为
	fontSetting.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&fontSetting);						// 设置字体样式


	time_t nowTime = time(&timep);
	time_t score = (nowTime - sratTimeStamp) + (time_t)(fedNum * 10);

	if (settings.speed < 6)
		settings.speed = (score / 100) + settings.speed;


	if (multi) {
		_stprintf(str, _T("Game Time:"));
		outtextxy(settings.width * settings.blockSize + 10, settings.height, str);

		_stprintf(str, _T("%d"), (nowTime - sratTimeStamp));
		outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize, str);

		_stprintf(str, _T("Speed:"));
		outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize * 2, str);

		_stprintf(str, _T("%d"),settings.speed);
		outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize * 3, str);

		return;
	}

	if (start) {
		getch();

		_stprintf(str, _T("按下任意键开始"));
		outtextxy(settings.width * settings.blockSize + 5, settings.height * settings.blockSize / 2, str);

		getch();

		setfillcolor(0xCAE1A4);
		solidrectangle(settings.width * settings.blockSize + 1, settings.height * settings.blockSize / 2, settings.width * settings.blockSize + 150, settings.height * settings.blockSize);
	}
	else {

		_stprintf(str, _T("Game Score:"));
		outtextxy(settings.width * settings.blockSize + 10, settings.height, str);


		_stprintf(str, _T("%d"), score);
		outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize, str);

		_stprintf(str, _T("Body Length:"));
		outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize * 2, str);


		_stprintf(str, _T("%d"), fedNum + 3);
		outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize * 3, str);

		_stprintf(str, _T("Speed:"));
		outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize * 4, str);

		_stprintf(str, _T("%d"), settings.speed);
		outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize * 5, str);

	}
	return;
}

void settingMenu() {

	cleardevice();

	ExMessage input;
	input = getmessage(EX_MOUSE | EX_KEY);

	LOGFONT fontSetting;
	gettextstyle(&fontSetting);
	TCHAR str[25];
	fontSetting.lfHeight = 48;
	settextstyle(&fontSetting);


	//读取设置



	_stprintf(str, _T("设置"));
	outtextxy(((settings.width * settings.blockSize) / 2) + settings.blockSize, (settings.blockSize) + settings.blockSize * 0.2, str);

	fontSetting.lfHeight = 27;
	settextstyle(&fontSetting);

	_stprintf(str, _T("按下对应字母选择按钮"));
	outtextxy(settings.width * settings.blockSize - settings.blockSize * 3, settings.blockSize * 2, str);


	fontSetting.lfHeight = 48;
	settextstyle(&fontSetting);

	_stprintf(str, _T("按钮"));
	outtextxy(settings.blockSize * 2, settings.blockSize * 4, str);


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

void saveSettingsBinary(const Setting* settings) {
	FILE* file = fopen("settings.bin", "wb");
	if (file != NULL) {
		fwrite(settings, sizeof(Setting), 1, file);
		fclose(file);
	}
	return;

}

void loadSettingsBinary(Setting* settings) {
	FILE* file = fopen("settings.bin", "rb");
	if (file != NULL) {
		fread(settings, sizeof(Setting), 1, file);
		fclose(file);
	}
	else {
		Setting newSettings = { {0},1,0,4,40,30,25 };
		saveSettingsBinary(&newSettings);
		loadSettingsBinary(settings);
	}
	return;
}
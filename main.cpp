#include <easyx.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <process.h>

#pragma warning(disable:4996)
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

void initStartMenu();
void startGame();
void startGameDouble();

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

void updateMap(snake* snakePoint, int x) {

	if (x) {

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
	//显示头和身1
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

void gameOver(int i) {
	//哪条蛇调用gameover 和 游戏人数
	ExMessage input;
	settextcolor(0xffffff);

	settextstyle(27, 0, _T("微软雅黑"));
	outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT * BLOCKSIZE / 2, L"GAME OVER");
	settextstyle(35, 0, _T("微软雅黑"));
	outtextxy(WIDTH * BLOCKSIZE + BLOCKSIZE, (HEIGHT * BLOCKSIZE) - (BLOCKSIZE * 2), L"重新开始");
	outtextxy(WIDTH * BLOCKSIZE + BLOCKSIZE, (HEIGHT * BLOCKSIZE) - (BLOCKSIZE * 4), L"返回菜单");
	
	
	if (i == 2) {
		WaitForSingleObject(mutex, INFINITE);
		
	}
	//释放蛇头1

	while (1)
	{
		// 获取按键信息
		input = getmessage(EX_MOUSE | EX_KEY);

		switch (input.message)
		{
		case WM_LBUTTONDOWN:

			if (((input.x >= (WIDTH * BLOCKSIZE + BLOCKSIZE)) && (input.x <= (WIDTH * BLOCKSIZE + BLOCKSIZE * 5))) && ((input.y >= ((HEIGHT * BLOCKSIZE) - (BLOCKSIZE * 2))) && (input.y <= ((HEIGHT * BLOCKSIZE) - (BLOCKSIZE * 0.8))))) {

				cleardevice();
				if (i == 1) {
					freeSnake(snakeHead);
					startGame();
					return;
				}
				else {
					freeSnake(snakeHeadP2);
					freeSnake(snakeHead);
					startGameDouble();
					return;

				}
					//释放蛇头2
				

				return;
				//重新开始
			}
			else if (((input.x >= (WIDTH * BLOCKSIZE + BLOCKSIZE)) && (input.x <= (WIDTH * BLOCKSIZE + BLOCKSIZE * 5))) && ((input.y >= ((HEIGHT * BLOCKSIZE) - (BLOCKSIZE * 4))) && (input.y <= ((HEIGHT * BLOCKSIZE) - (BLOCKSIZE * 2) - (BLOCKSIZE * 0.8))))) {
				
				if (i == 2){
					freeSnake(snakeHeadP2);

					ReleaseMutex(mutex);
				}
				//释放蛇头1
				cleardevice();

				initStartMenu();
				return;
			//返回开始菜单
			}

			break;

		case WM_KEYDOWN:
			if (input.vkcode == VK_ESCAPE) {
				cleardevice();
				initStartMenu();

				//返回开始菜单
			}
		}
	}
	return;

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

void gameBar(bool start,bool multi) {

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

void game()
{

	char input = 'd', lastInput = 'd';
	int x = 0, y = 0;

	while (1)
	{
		snake* snakePoint = (snake*)malloc(sizeof(snake));
		snakePoint->next = snakeHead;
		snakePoint->x = snakeHead->x;
		snakePoint->y = snakeHead->y;

		Sleep(1000 / speed);

		/*	if (KEY_DOWN(65))
				input = 'a';
			else if (KEY_DOWN(87))
				input = 'w';
			else if (KEY_DOWN(83))
				input = 's';
			else if (KEY_DOWN(68))
				input = 'd';*/


		if (kbhit() != 0)
		{
			while (!(kbhit() == 0))
				input = getch();
		}

		if (input == 'z') {
			input = input;
		}

		if (input != 'w' && input != 'a' && input != 's' && input != 'd') {
			input = lastInput;
		}


		switch (input)
		{


		case 'a':
			if (lastInput == 'd') {
				snakePoint->x += 1;
				input = 'd';
				break;
			}
			else
			{
				snakePoint->x -= 1;
				break;
			}

		case 'w':
			if (lastInput == 's') {
				snakePoint->y += 1;

				input = 's';
				break;
			}
			else {


				snakePoint->y -= 1;
				break;
			}

		case 'd':
			if (lastInput == 'a') {
				snakePoint->x -= 1;
				input = 'a';
				break;
			}
			else {
				snakePoint->x += 1;
				break;
			}

		case 's':
			if (lastInput == 'w') {
				snakePoint->y -= 1;
				input = 'w';
				break;
			}
			else {
				snakePoint->y += 1;
				break;
			}

		default:
			break;
		}

		updateMap(snakeHead, 0);

		if (map[snakePoint->y][snakePoint->x] == '*') {

			snake* snakePoints;

			for (snakePoints = snakeHead; snakePoints->next; snakePoints = snakePoints->next)
			{
				if (snakePoints == snakeHead) {
					setfillcolor(0xffffff);
					solidrectangle(((snakePoints->x * BLOCKSIZE) + 1), ((snakePoints->y * BLOCKSIZE) + 1), (((snakePoints->x + 1) * BLOCKSIZE) - 1), (((snakePoints->y + 1) * BLOCKSIZE) - 1));

				}
			}
			fed = 0;
			map[snakePoint->y][snakePoint->x] = ' ';
		}
		else {

			hideSnake(snakeHead, 0xffffff);//删除蛇头和蛇尾显示
			updateSnake(snakeHead);//删除蛇尾
		}

		snakeHead = snakePoint;
		showSnake(snakeHead, RED);//显示新蛇头
		updateMap(snakeHead, 1);
		food();
		gameBar(0,0);

		if (isDeath(snakeHead, NULL)) {
			return;
		}


		lastInput = input;
	}
}

DWORD WINAPI gameP1(LPVOID pm) {
	char input = 'd', lastInput = 'd';
	drawSnake drawsnake;
	while (bSnakeP1Continue)
	{

		snake* snakePoint = (snake*)malloc(sizeof(snake));
		snakePoint->next = snakeHead;
		snakePoint->x = snakeHead->x;
		snakePoint->y = snakeHead->y;

		Sleep(1000 / speed);

		if (KEY_DOWN(65))
			input = 'a';
		else if (KEY_DOWN(87))
			input = 'w';
		else if (KEY_DOWN(83))
			input = 's';
		else if (KEY_DOWN(68))
			input = 'd';
		else if (KEY_DOWN('Z'))
			input = input;

		/*if (input != 'w' && input != 'a' && input != 's' && input != 'd') {
			input = lastInput;
		}*/


		switch (input)
		{


		case 'a':
			if (lastInput == 'd') {
				snakePoint->x += 1;
				input = 'd';
				break;
			}
			else
			{
				snakePoint->x -= 1;
				break;
			}

		case 'w':
			if (lastInput == 's') {
				snakePoint->y += 1;

				input = 's';
				break;
			}
			else {


				snakePoint->y -= 1;
				break;
			}

		case 'd':
			if (lastInput == 'a') {
				snakePoint->x -= 1;
				input = 'a';
				break;
			}
			else {
				snakePoint->x += 1;
				break;
			}

		case 's':
			if (lastInput == 'w') {
				snakePoint->y -= 1;
				input = 'w';
				break;
			}
			else {
				snakePoint->y += 1;
				break;
			}

		default:
			break;
		}

		updateMap(snakeHead, 0);
		if (map[snakePoint->y][snakePoint->x] == '*') {

			snake* snakePoints;

			for (snakePoints = snakeHead; snakePoints->next; snakePoints = snakePoints->next)
			{
				if (snakePoints == snakeHead) {
					/*drawsnake = { 0xffffff,snakePoint };
					CreateThread(NULL, 0, drawSnakeBody, &drawsnake, 0, NULL);*/

					WaitForSingleObject(mutex, INFINITE);

					setfillcolor(0xffffff);
					solidrectangle(((snakePoints->x * BLOCKSIZE) + 1), ((snakePoints->y * BLOCKSIZE) + 1), (((snakePoints->x + 1) * BLOCKSIZE) - 1), (((snakePoints->y + 1) * BLOCKSIZE) - 1));

					ReleaseMutex(mutex);
					//Sleep(100);

				}
			}
			WaitForSingleObject(mutex, INFINITE);

			fed = 0;
			map[snakePoint->y][snakePoint->x] = ' ';
			ReleaseMutex(mutex);

		}
		else {
			WaitForSingleObject(mutex, INFINITE);

			hideSnake(snakeHead, 0xffffff);//删除蛇头和蛇尾显示
			//Sleep(100);

			updateSnake(snakeHead);//删除蛇尾

			ReleaseMutex(mutex);
			//Sleep(100);
		}
		snakeHead = snakePoint;

		WaitForSingleObject(mutex, INFINITE);

		showSnake(snakeHead, RED);
		//Sleep(100);

		updateMap(snakeHead, 1);
		//Sleep(100);
		food();

		gameBar(0,1);

		ReleaseMutex(mutex);


		WaitForSingleObject(mutex, INFINITE);

		if (isDeath(snakeHead, snakeHeadP2)) {
			return 0;
		}

		ReleaseMutex(mutex);

		lastInput = input;
	}
	return 0;
 }

DWORD WINAPI gameP2(LPVOID pm) {
	int  inputP2 = RIGHT, lastInputP2 = RIGHT;
	while (bSnakeP2Continue)
	{

		snake* snakePointP2 = (snake*)malloc(sizeof(snake));
		snakePointP2->next = snakeHeadP2;
		snakePointP2->x = snakeHeadP2->x;
		snakePointP2->y = snakeHeadP2->y;


		Sleep(1000 / speed);


		if (KEY_DOWN(LEFT))
			inputP2 = LEFT;
		else if (KEY_DOWN(RIGHT))
			inputP2 = RIGHT;
		else if (KEY_DOWN(UP))
			inputP2 = UP;
		else if (KEY_DOWN(DOWN))
			inputP2 = DOWN;
		else if (KEY_DOWN('Z'))
			inputP2 = inputP2;


		switch (inputP2)
		{


		case LEFT:
			if (lastInputP2 == RIGHT) {
				snakePointP2->x += 1;
				inputP2 = RIGHT;
				break;
			}
			else
			{
				snakePointP2->x -= 1;
				break;
			}

		case UP:
			if (lastInputP2 == DOWN) {
				snakePointP2->y += 1;

				inputP2 = DOWN;
				break;
			}
			else {
				snakePointP2->y -= 1;
				break;
			}

		case RIGHT:
			if (lastInputP2 == LEFT) {
				snakePointP2->x -= 1;
				inputP2 = LEFT;
				break;
			}
			else {
				snakePointP2->x += 1;
				break;
			}

		case DOWN:
			if (lastInputP2 == UP) {
				snakePointP2->y -= 1;
				inputP2 = UP;
				break;
			}
			else {
				snakePointP2->y += 1;
				break;
			}
		default:
			break;
		}


		updateMap(snakeHeadP2, 0);
		//Sleep(100);
		if (map[snakePointP2->y][snakePointP2->x] == '*') {

			snake* snakePointsP2;

			for (snakePointsP2 = snakeHeadP2; snakePointsP2->next; snakePointsP2 = snakePointsP2->next)
			{
				if (snakePointsP2 == snakeHeadP2) {

					WaitForSingleObject(mutex, INFINITE);
					//setfillcolor(0xFC5633);

					setfillcolor(0xFC5633);
					solidrectangle(((snakePointsP2->x * BLOCKSIZE) + 1), ((snakePointsP2->y * BLOCKSIZE) + 1), (((snakePointsP2->x + 1) * BLOCKSIZE) - 1), (((snakePointsP2->y + 1) * BLOCKSIZE) - 1));
					ReleaseMutex(mutex);
					//Sleep(100);
				}
			}

			WaitForSingleObject(mutex, INFINITE);

			fed = 0;
			map[snakePointP2->y][snakePointP2->x] = ' ';

			ReleaseMutex(mutex);

		}
		else {

			WaitForSingleObject(mutex, INFINITE);

			hideSnake(snakeHeadP2, 0xFC5633);//删除蛇头和蛇尾显示
			//Sleep(100);
			updateSnake(snakeHeadP2);//删除蛇尾
			//Sleep(100);

			ReleaseMutex(mutex);

		}
		snakeHeadP2 = snakePointP2;

		WaitForSingleObject(mutex, INFINITE);

		showSnake(snakeHeadP2, 0x000000);
		//showSnakeP2();

		updateMap(snakeHeadP2, 1);
		//Sleep(100);


		food();

		gameBar(0,1);

		ReleaseMutex(mutex);


		WaitForSingleObject(mutex, INFINITE);

		if (isDeath(snakeHeadP2, snakeHead)) {
			return 0;
		}
		ReleaseMutex(mutex);


		lastInputP2 = inputP2;
	}
}



void startGame() {

	sratTimeStamp = time(&timep);

	srand(sratTimeStamp);

	fed = 0;
	fedNum = -1;
	speed = SPEED;
	initMap();
	initLine();
	initSnake(1);
	gameBar(1,0);
	game();
	return;
}

void startGameDouble() {

	sratTimeStamp = time(&timep);

	srand(sratTimeStamp);

	fed = 0;
	fedNum = -1;
	speed = SPEED;

	initMap();
	initLine();
	initSnake(2);

	mutex = CreateMutex(NULL, FALSE, NULL);

	if (gameHandleP1 != NULL){
		CloseHandle(gameHandleP1);
		gameHandleP1 = NULL;
	}
	if (gameHandleP2 != NULL){
		CloseHandle(gameHandleP2);
		gameHandleP2 = NULL;

	}
	bSnakeP1Continue = 1;
	bSnakeP2Continue = 1;

	gameHandleP1 = CreateThread(NULL, 0, gameP1, NULL, 1, NULL);
	gameHandleP2 = CreateThread(NULL, 0, gameP2, NULL, 1, NULL);

	WaitForSingleObject(gameHandleP1, INFINITE);
	WaitForSingleObject(gameHandleP2, INFINITE);

	bSnakeP1Continue = 0;
	bSnakeP2Continue = 0;
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

void initStartMenu() {

	cleardevice();
	ExMessage input;
	TCHAR str[BLOCKSIZE];
	LOGFONT fontSetting;

	gettextstyle(&fontSetting);						// 获取当前字体设置
	fontSetting.lfHeight = 100;						// 设置字体高度
	fontSetting.lfWeight = 700;						// 设置字体粗细
	_tcscpy(fontSetting.lfFaceName, _T("微软雅黑"));		// 设置字体
	fontSetting.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&fontSetting);						// 设置字体样式
	settextcolor(0xffffff);


	_stprintf(str, _T("贪吃蛇"));
	outtextxy(((WIDTH * BLOCKSIZE) / 2) - BLOCKSIZE * 2, (HEIGHT * BLOCKSIZE) / 3, str);


	fontSetting.lfHeight = 45;						// 设置字体高度
	settextstyle(&fontSetting);						// 设置字体样式

	//(HEIGHT * BLOCKSIZE)*0.5
	/*fillrectangle(((WIDTH * BLOCKSIZE) / 4.5), (HEIGHT * BLOCKSIZE) * 0.7, ((WIDTH * BLOCKSIZE) / 4.5)+(BLOCKSIZE*5), ((HEIGHT * BLOCKSIZE) * 0.8));
	fillrectangle(((WIDTH * BLOCKSIZE) / 2)+ BLOCKSIZE/2, (HEIGHT * BLOCKSIZE) * 0.7, ((WIDTH * BLOCKSIZE) / 2) + (BLOCKSIZE * 6)- BLOCKSIZE / 2, ((HEIGHT * BLOCKSIZE) * 0.8));
	fillrectangle(((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE, (HEIGHT * BLOCKSIZE) * 0.7, ((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE + (BLOCKSIZE * 5), ((HEIGHT * BLOCKSIZE) * 0.8));*/

	_stprintf(str, _T("开始游戏"));
	outtextxy(((WIDTH * BLOCKSIZE) / 4.5), (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2, str);

	_stprintf(str, _T("设置"));
	outtextxy(((WIDTH * BLOCKSIZE) / 2) + BLOCKSIZE, (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2, str);

	_stprintf(str, _T("退出游戏"));
	outtextxy(((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE, (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2, str);

	_stprintf(str, _T("双人游戏"));
	outtextxy(((WIDTH * BLOCKSIZE) / 4.5), (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 4, str);

	//fillrectangle(((WIDTH * BLOCKSIZE) / 4.5), (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 4, ((WIDTH * BLOCKSIZE) / 4.5)+(BLOCKSIZE*6), ((HEIGHT * BLOCKSIZE) * 0.78 + BLOCKSIZE * 4));


	while (1)
	{
		// 获取按键信息
		input = getmessage(EX_MOUSE | EX_KEY);

		switch (input.message)
		{
		case WM_LBUTTONDOWN:

			if ((input.x >= ((WIDTH * BLOCKSIZE) / 4.5) && (input.x <= ((WIDTH * BLOCKSIZE) / 4.5) + (BLOCKSIZE * 6))) && ((input.y >= (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2) && (input.y <= ((HEIGHT * BLOCKSIZE) * 0.8) + BLOCKSIZE * 0.2))) {
				/*_stprintf(str, _T("x:%05d"), input.x);
				outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE, str);

				_stprintf(str, _T("y:%05d"), input.y);
				outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 2, str);*/
				cleardevice();
				startGame();
				return;
			}
			else if (((input.x >= ((WIDTH * BLOCKSIZE) / 2) + BLOCKSIZE / 2) && input.x <= (((WIDTH * BLOCKSIZE) / 2) + (BLOCKSIZE * 6) - BLOCKSIZE / 2)) && ((input.y >= ((HEIGHT * BLOCKSIZE) * 0.7) && (input.y <= ((HEIGHT * BLOCKSIZE) * 0.8))))) {
				settingMenu();
				initStartMenu();
			}
			else if (((input.x >= ((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE) && (input.x <= ((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE + (BLOCKSIZE * 5))) && ((input.y >= (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2) && (input.y <= ((HEIGHT * BLOCKSIZE) * 0.8) + BLOCKSIZE * 0.2))) {
				exit(0);
			}
			else if ((input.x >= ((WIDTH * BLOCKSIZE) / 4.5) && (input.x <= ((WIDTH * BLOCKSIZE) / 4.5) + (BLOCKSIZE * 6))) && ((input.y >= (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 4) && (input.y <= ((HEIGHT * BLOCKSIZE) * 0.78) + BLOCKSIZE * 4))) {
				cleardevice();
				startGameDouble();
				return;
			}

		case WM_KEYDOWN:
			if (input.vkcode == VK_ESCAPE)
				return;	// 按 ESC 键退出程序
		}
	}


}

int main()
{
	initWindow();
	initStartMenu();
	closegraph();

	return 0;
}
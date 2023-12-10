#include "main.h"


void game()
{

	char input = 'd', lastInput = 'd';
	int x = 0, y = 0;
	int a;
	while (1)
	{
		snake* snakePoint = (snake*)malloc(sizeof(snake));
		snakePoint->next = snakeHead;
		snakePoint->x = snakeHead->x;
		snakePoint->y = snakeHead->y;

		Sleep(1000 / settings.speed);

		/*	if (KEY_DOWN(65))
				input = 'a';
			else if (KEY_DOWN(87))
				input = 'w';
			else if (KEY_DOWN(83))
				input = 's';
			else if (KEY_DOWN(68))
				input = 'd';*/

		//if ((a=kbhit()) != 0)
		//{
		//	while (!(kbhit() == 0))
		//		input = getch();
		//}

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
					solidrectangle(((snakePoints->x * settings.blockSize) + 1), ((snakePoints->y * settings.blockSize) + 1), (((snakePoints->x + 1) * settings.blockSize) - 1), (((snakePoints->y + 1) * settings.blockSize) - 1));

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
	while (bSnakeP1Continue)
	{

		snake* snakePoint = (snake*)malloc(sizeof(snake));
		snakePoint->next = snakeHead;
		snakePoint->x = snakeHead->x;
		snakePoint->y = snakeHead->y;

		Sleep(1000 / settings.speed);

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
					solidrectangle(((snakePoints->x * settings.blockSize) + 1), ((snakePoints->y * settings.blockSize) + 1), (((snakePoints->x + 1) * settings.blockSize) - 1), (((snakePoints->y + 1) * settings.blockSize) - 1));

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

		Sleep(1000 / settings.speed);

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
					solidrectangle(((snakePointsP2->x * settings.blockSize) + 1), ((snakePointsP2->y * settings.blockSize) + 1), (((snakePointsP2->x + 1) * settings.blockSize) - 1), (((snakePointsP2->y + 1) * settings.blockSize) - 1));
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
	newMap(settings.width, settings.height);
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

	newMap(settings.width, settings.height);
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


//初始化开始菜单
void initStartMenu() {


	cleardevice();


	ExMessage input;
	TCHAR str[25];
	LOGFONT fontSetting;

	gettextstyle(&fontSetting);						// 获取当前字体设置
	fontSetting.lfHeight = 100;						// 设置字体高度
	fontSetting.lfWeight = 700;						// 设置字体粗细
	_tcscpy(fontSetting.lfFaceName, _T("微软雅黑"));		// 设置字体
	fontSetting.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&fontSetting);						// 设置字体样式
	settextcolor(0xffffff);


	_stprintf(str, _T("贪吃蛇"));
	outtextxy(((settings.width * settings.blockSize) / 2) - settings.blockSize * 2, (settings.height * settings.blockSize) / 3, str);


	fontSetting.lfHeight = 45;						// 设置字体高度
	settextstyle(&fontSetting);						// 设置字体样式

	//(settings.height * settings.blockSize)*0.5
	/*fillrectangle(((settings.width * settings.blockSize) / 4.5), (settings.height * settings.blockSize) * 0.7, ((settings.width * settings.blockSize) / 4.5)+(settings.blockSize*5), ((settings.height * settings.blockSize) * 0.8));
	fillrectangle(((settings.width * settings.blockSize) / 2)+ settings.blockSize/2, (settings.height * settings.blockSize) * 0.7, ((settings.width * settings.blockSize) / 2) + (settings.blockSize * 6)- settings.blockSize / 2, ((settings.height * settings.blockSize) * 0.8));
	fillrectangle(((settings.width * settings.blockSize) / 1.2) - settings.blockSize, (settings.height * settings.blockSize) * 0.7, ((settings.width * settings.blockSize) / 1.2) - settings.blockSize + (settings.blockSize * 5), ((settings.height * settings.blockSize) * 0.8));*/

	_stprintf(str, _T("开始游戏"));
	outtextxy(((settings.width * settings.blockSize) / 4.5), (settings.height * settings.blockSize) * 0.7 + settings.blockSize * 0.2, str);

	_stprintf(str, _T("设置"));
	outtextxy(((settings.width * settings.blockSize) / 2) + settings.blockSize, (settings.height * settings.blockSize) * 0.7 + settings.blockSize * 0.2, str);

	_stprintf(str, _T("退出游戏"));
	outtextxy(((settings.width * settings.blockSize) / 1.2) - settings.blockSize, (settings.height * settings.blockSize) * 0.7 + settings.blockSize * 0.2, str);

	_stprintf(str, _T("双人游戏"));
	outtextxy(((settings.width * settings.blockSize) / 4.5), (settings.height * settings.blockSize) * 0.7 + settings.blockSize * 4, str);

	//fillrectangle(((settings.width * settings.blockSize) / 4.5), (settings.height * settings.blockSize) * 0.7 + settings.blockSize * 4, ((settings.width * settings.blockSize) / 4.5)+(settings.blockSize*6), ((settings.height * settings.blockSize) * 0.78 + settings.blockSize * 4));


	while (1)
	{
		// 获取按键信息
		input = getmessage(EX_MOUSE | EX_KEY);

		switch (input.message)
		{
		case WM_LBUTTONDOWN:

			if ((input.x >= ((settings.width * settings.blockSize) / 4.5) && (input.x <= ((settings.width * settings.blockSize) / 4.5) + (settings.blockSize * 6))) && ((input.y >= (settings.height * settings.blockSize) * 0.7 + settings.blockSize * 0.2) && (input.y <= ((settings.height * settings.blockSize) * 0.8) + settings.blockSize * 0.2))) {
				/*_stprintf(str, _T("x:%05d"), input.x);
				outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize, str);

				_stprintf(str, _T("y:%05d"), input.y);
				outtextxy(settings.width * settings.blockSize + 10, settings.height + settings.blockSize * 2, str);*/
				cleardevice();
				startGame();
			}
			else if (((input.x >= ((settings.width * settings.blockSize) / 2) + settings.blockSize / 2) && input.x <= (((settings.width * settings.blockSize) / 2) + (settings.blockSize * 6) - settings.blockSize / 2)) && ((input.y >= ((settings.height * settings.blockSize) * 0.7) && (input.y <= ((settings.height * settings.blockSize) * 0.8))))) {
				settingMenu();
				initStartMenu();
				//开始单人游戏
			}
			else if (((input.x >= ((settings.width * settings.blockSize) / 1.2) - settings.blockSize) && (input.x <= ((settings.width * settings.blockSize) / 1.2) - settings.blockSize + (settings.blockSize * 5))) && ((input.y >= (settings.height * settings.blockSize) * 0.7 + settings.blockSize * 0.2) && (input.y <= ((settings.height * settings.blockSize) * 0.8) + settings.blockSize * 0.2))) {
				exit(0);
				return;
				//结束游戏
			}
			else if ((input.x >= ((settings.width * settings.blockSize) / 4.5) && (input.x <= ((settings.width * settings.blockSize) / 4.5) + (settings.blockSize * 6))) && ((input.y >= (settings.height * settings.blockSize) * 0.7 + settings.blockSize * 4) && (input.y <= ((settings.height * settings.blockSize) * 0.78) + settings.blockSize * 4))) {
				cleardevice();
				startGameDouble();
				//开始双人游戏
			}

		case WM_KEYDOWN:
			if (input.vkcode == VK_ESCAPE)
				return;	// 按 ESC 键退出程序
		}
	}

	return;

}


//游戏结束 传入是几个人游戏
void gameOver(int i) {
	//i为游戏人数
	ExMessage input;
	settextcolor(0xffffff);

	settextstyle(27, 0, _T("微软雅黑"));
	outtextxy(settings.width * settings.blockSize + 10, settings.height * settings.blockSize / 2, L"GAME OVER");
	settextstyle(35, 0, _T("微软雅黑"));
	outtextxy(settings.width * settings.blockSize + settings.blockSize, (settings.height * settings.blockSize) - (settings.blockSize * 2), L"重新开始");
	outtextxy(settings.width * settings.blockSize + settings.blockSize, (settings.height * settings.blockSize) - (settings.blockSize * 4), L"返回菜单");


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

			if (((input.x >= (settings.width * settings.blockSize + settings.blockSize)) && (input.x <= (settings.width * settings.blockSize + settings.blockSize * 5))) && ((input.y >= ((settings.height * settings.blockSize) - (settings.blockSize * 2))) && (input.y <= ((settings.height * settings.blockSize) - (settings.blockSize * 0.8))))) {

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
			else if (((input.x >= (settings.width * settings.blockSize + settings.blockSize)) && (input.x <= (settings.width * settings.blockSize + settings.blockSize * 5))) && ((input.y >= ((settings.height * settings.blockSize) - (settings.blockSize * 4))) && (input.y <= ((settings.height * settings.blockSize) - (settings.blockSize * 2) - (settings.blockSize * 0.8))))) {

				if (i == 2) {
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


int main()
{
	loadSettingsBinary(&settings);
	initWindow();
	initStartMenu();
	closegraph();

	return 0;
}
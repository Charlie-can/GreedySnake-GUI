#include "main.h"


void game()
{

	//初始向右前进
	char input = 'd', lastInput = 'd';
	int x = 0, y = 0;

	while (1)
	{
		//生成下一个蛇身
		snake* snakePoint = (snake*)malloc(sizeof(snake));
		snakePoint->next = snakeHead;
		snakePoint->x = snakeHead->x;
		snakePoint->y = snakeHead->y;


		//设定刷新时间
		Sleep(1000 / speed);

		/*	if (KEY_DOWN(65))
				input = 'a';
			else if (KEY_DOWN(87))
				input = 'w';
			else if (KEY_DOWN(83))
				input = 's';
			else if (KEY_DOWN(68))
				input = 'd';*/

		//如果按下按键
		if (kbhit() != 0)
		{
			while (!(kbhit() == 0))
				input = getch();
		}

		if (input == 'z') {
			input = input;
		}
		// 判断按键是否冲突
		if (input != 'w' && input != 'a' && input != 's' && input != 'd') {
			input = lastInput;
		}

		//判断按下按键
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

		//更新蛇在地图数组中的位置
		updateMap(snakeHead, 0);

		//如果吃下食物
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
			//没有吃到食物

			hideSnake(snakeHead, 0xffffff);//删除蛇头和蛇尾显示
			updateSnake(snakeHead);//删除蛇尾
		}

		snakeHead = snakePoint;
		showSnake(snakeHead, RED);//显示新蛇头
		updateMap(snakeHead, 1);//更新蛇在地图的位置
		food();	//进入食物函数刷新食物
		gameBar(0,0);	//刷新侧边栏

		//判断是否死亡
		if (isDeath(snakeHead, NULL)) {
			return;
		}


		lastInput = input;
	}
}

DWORD WINAPI gameP1(LPVOID pm ) {
	char input = 'd', lastInput = 'd';
	drawSnake drawsnake;
	lengthP1 = 3;
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

		if (KEY_DOWN('J'))
			Sleep(100);



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


		if (additionP1 > 0) {

			additionP1--;
			snake* snakePoints;

			for (snakePoints = snakeHead; snakePoints->next; snakePoints = snakePoints->next)
			{
				if (snakePoints == snakeHead) {

					WaitForSingleObject(mutex, INFINITE);

					setfillcolor(0xffffff);
					solidrectangle(((snakePoints->x * BLOCKSIZE) + 1), ((snakePoints->y * BLOCKSIZE) + 1), (((snakePoints->x + 1) * BLOCKSIZE) - 1), (((snakePoints->y + 1) * BLOCKSIZE) - 1));
					lengthP1++;
					ReleaseMutex(mutex);
					//Sleep(100);

				}
			}
		}
		else if (map[snakePoint->y][snakePoint->x] == '*') {


			snake* snakePoints;

			for (snakePoints = snakeHead; snakePoints->next; snakePoints = snakePoints->next)
			{
				if (snakePoints == snakeHead) {
					/*drawsnake = { 0xffffff,snakePoint };
					CreateThread(NULL, 0, drawSnakeBody, &drawsnake, 0, NULL);*/

					WaitForSingleObject(mutex, INFINITE);

					setfillcolor(0xffffff);
					solidrectangle(((snakePoints->x * BLOCKSIZE) + 1), ((snakePoints->y * BLOCKSIZE) + 1), (((snakePoints->x + 1) * BLOCKSIZE) - 1), (((snakePoints->y + 1) * BLOCKSIZE) - 1));
					lengthP1++;
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

		if (isDeath(snakeHead, snakeHeadP2)==2) {
			clearSnake(snakeHead);
			freeSnake(snakeHead);
			initSnake(1, 0);
			additionP2 += lengthP1-1;
			additionP1 = 0;
			ReleaseMutex(mutex);
			gameHandleP1 = CreateThread(NULL, 0, gameP1, NULL, 1, NULL);
			WaitForSingleObject(gameHandleP1, INFINITE);
			return 0;

		}else if (isDeath(snakeHead, snakeHeadP2)) {
			 return 0;
		}

		ReleaseMutex(mutex);

		lastInput = input;
	}
	return 0;
 }

DWORD WINAPI gameP2(LPVOID pm) {
	int  inputP2 = RIGHT, lastInputP2 = RIGHT;
	lengthP2 = 3;

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
		
		if (KEY_DOWN(96))
			Sleep(100);


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


		if (additionP2 > 0) {
			additionP2--;

			snake* snakePointsP2;

			for (snakePointsP2 = snakeHeadP2; snakePointsP2->next; snakePointsP2 = snakePointsP2->next)
			{
				if (snakePointsP2 == snakeHeadP2) {

					WaitForSingleObject(mutex, INFINITE);
					//setfillcolor(0xFC5633);

					setfillcolor(0xFC5633);
					solidrectangle(((snakePointsP2->x * BLOCKSIZE) + 1), ((snakePointsP2->y * BLOCKSIZE) + 1), (((snakePointsP2->x + 1) * BLOCKSIZE) - 1), (((snakePointsP2->y + 1) * BLOCKSIZE) - 1));
					lengthP2++;
					ReleaseMutex(mutex);
					//Sleep(100);
				}
			}


		}
		else if (map[snakePointP2->y][snakePointP2->x] == '*') {

			snake* snakePointsP2;

			for (snakePointsP2 = snakeHeadP2; snakePointsP2->next; snakePointsP2 = snakePointsP2->next)
			{
				if (snakePointsP2 == snakeHeadP2) {

					WaitForSingleObject(mutex, INFINITE);
					//setfillcolor(0xFC5633);

					setfillcolor(0xFC5633);
					solidrectangle(((snakePointsP2->x * BLOCKSIZE) + 1), ((snakePointsP2->y * BLOCKSIZE) + 1), (((snakePointsP2->x + 1) * BLOCKSIZE) - 1), (((snakePointsP2->y + 1) * BLOCKSIZE) - 1));
					lengthP2++;
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
		
		{
			 if(isDeath(snakeHeadP2, snakeHead)==2){
				 clearSnake(snakeHeadP2);
				 freeSnake(snakeHeadP2);
				 initSnake(0, 1);
				 additionP1 += lengthP2-1;
				 additionP2 = 0;
				 ReleaseMutex(mutex);
				 gameHandleP2 = CreateThread(NULL, 0, gameP2, NULL, 1, NULL);
				 WaitForSingleObject(gameHandleP2, INFINITE);
				 return 0;
			 }

		} if (isDeath(snakeHeadP2, snakeHead)) {
			return 0;
		}
		ReleaseMutex(mutex);


		lastInputP2 = inputP2;
	}
}

void startGame() {
	//创建时间戳记录开始时间
	sratTimeStamp = time(&timep);
	//随机函数
	srand(sratTimeStamp);

	fed = 0;
	fedNum = -1;
	speed = SPEED;
	//初始化地图
	initMap();
	//初始化地图框线
	initLine();
	//初始化蛇
	initSnake(1,0);
	//初始化侧边栏
	gameBar(1,0);
	//开始游戏
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
	initSnake(1,1);

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

void initStartMenu() {

	

	//设置字体等信息
	isCombat = 0;
	cleardevice();
	ExMessage input;
	TCHAR str[BLOCKSIZE];
	LOGFONT fontSetting;

	gettextstyle(&fontSetting);						// 获取当前字体设置
	fontSetting.lfHeight = BLOCKSIZE*4;						// 设置字体高度
	fontSetting.lfWeight = 700;						// 设置字体粗细
	_tcscpy(fontSetting.lfFaceName, _T("微软雅黑"));		// 设置字体
	fontSetting.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&fontSetting);						// 设置字体样式
	settextcolor(0xffffff);

	//输出文字

	_stprintf(str, _T("贪吃蛇"));
	outtextxy(((WIDTH * BLOCKSIZE) / 2) - BLOCKSIZE * 2, (HEIGHT * BLOCKSIZE) / 3, str);


	fontSetting.lfHeight = BLOCKSIZE*2;						// 设置字体高度
	settextstyle(&fontSetting);						// 设置字体样式

	//(HEIGHT * BLOCKSIZE)*0.5
	/*fillrectangle(((WIDTH * BLOCKSIZE) / 4.5), (HEIGHT * BLOCKSIZE) * 0.7, ((WIDTH * BLOCKSIZE) / 4.5)+(BLOCKSIZE*5), ((HEIGHT * BLOCKSIZE) * 0.8));
	fillrectangle(((WIDTH * BLOCKSIZE) / 2)+ BLOCKSIZE/2, (HEIGHT * BLOCKSIZE) * 0.7, ((WIDTH * BLOCKSIZE) / 2) + (BLOCKSIZE * 6)- BLOCKSIZE / 2, ((HEIGHT * BLOCKSIZE) * 0.8));
	fillrectangle(((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE, (HEIGHT * BLOCKSIZE) * 0.7, ((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE + (BLOCKSIZE * 5), ((HEIGHT * BLOCKSIZE) * 0.8));*/

	_stprintf(str, _T("单人游戏"));
	outtextxy(((WIDTH * BLOCKSIZE) / 4.5), (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2, str);

	//rectangle(((WIDTH * BLOCKSIZE) / 4.5) + (BLOCKSIZE * 6), ((HEIGHT * BLOCKSIZE) * 0.8) + BLOCKSIZE * 0.2, ((WIDTH * BLOCKSIZE) / 4.5),((HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2));


	_stprintf(str, _T("设置"));
	outtextxy(((WIDTH * BLOCKSIZE) / 2) + BLOCKSIZE, (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2, str);

	_stprintf(str, _T("退出游戏"));
	outtextxy(((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE, (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2, str);

	_stprintf(str, _T("双人游戏"));
	outtextxy(((WIDTH * BLOCKSIZE) / 4.5), (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 4, str);

	/*_stprintf(str, _T("蛇蛇争霸"));
	outtextxy(((WIDTH * BLOCKSIZE) / 2- WIDTH*0.15), (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 4, str);*/

	//fillrectangle(((WIDTH * BLOCKSIZE) / 4.5), (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 4, ((WIDTH * BLOCKSIZE) / 4.5)+(BLOCKSIZE*6), ((HEIGHT * BLOCKSIZE) * 0.78 + BLOCKSIZE * 4));
	//rectangle(((WIDTH * BLOCKSIZE) / 2.8) + (BLOCKSIZE * 6), ((HEIGHT * BLOCKSIZE) * 0.8) + BLOCKSIZE *2, ((WIDTH * BLOCKSIZE) / 1.6), (HEIGHT * BLOCKSIZE) * 0.8 + BLOCKSIZE * 0.2);



	while (1)
	{
		// 获取按键信息
		input = getmessage(EX_MOUSE | EX_KEY);
		//_stprintf(str, _T("x:%05d"), input.x);
		//		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE, str);

		//		_stprintf(str, _T("y:%05d"), input.y);
		//		outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT + BLOCKSIZE * 2, str);
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
				//单人游戏
			}
			else if (((input.x >= ((WIDTH * BLOCKSIZE) / 2) + BLOCKSIZE / 2) && input.x <= (((WIDTH * BLOCKSIZE) / 2) + (BLOCKSIZE * 6) - BLOCKSIZE / 2)) && ((input.y >= ((HEIGHT * BLOCKSIZE) * 0.7) && (input.y <= ((HEIGHT * BLOCKSIZE) * 0.8))))) {
				settingMenu();
				initStartMenu();
			}
			//设置菜单
			else if (((input.x >= ((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE) && (input.x <= ((WIDTH * BLOCKSIZE) / 1.2) - BLOCKSIZE + (BLOCKSIZE * 5))) && ((input.y >= (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 0.2) && (input.y <= ((HEIGHT * BLOCKSIZE) * 0.8) + BLOCKSIZE * 0.2))) {
				exit(0);
			}
			//退出游戏
			else if ((input.x >= ((WIDTH * BLOCKSIZE) / 4.5) && (input.x <= ((WIDTH * BLOCKSIZE) / 4.5) + (BLOCKSIZE * 6))) && ((input.y >= (HEIGHT * BLOCKSIZE) * 0.7 + BLOCKSIZE * 4) && (input.y <= ((HEIGHT * BLOCKSIZE) * 0.78) + BLOCKSIZE * 4))) {
				cleardevice();
				isCombat = 0;
				startGameDouble();
				return;
			}
			//多人游戏
			else if (((input.x >= ((WIDTH * BLOCKSIZE) / 2) + BLOCKSIZE / 2) && input.x <= (((WIDTH * BLOCKSIZE) / 2) + (BLOCKSIZE * 6) - BLOCKSIZE / 2)) && ((input.y >= ((HEIGHT * BLOCKSIZE) * 0.8 + BLOCKSIZE * 0.2) && (input.y <= ((HEIGHT * BLOCKSIZE) * 0.8) + BLOCKSIZE * 2)))){
				cleardevice();
				isCombat = 1;
				startGameDouble();

				return;
			}
			
					
		case WM_KEYDOWN:
			if (input.vkcode == VK_ESCAPE)
				return;	// 按 ESC 键退出程序
		}
	}

}

void gameOver(int x) {
	// 游戏人数
	ExMessage input;
	settextcolor(0xffffff);

	settextstyle(27, 0, _T("微软雅黑"));
	outtextxy(WIDTH * BLOCKSIZE + 10, HEIGHT * BLOCKSIZE / 2, L"GAME OVER");
	settextstyle(35, 0, _T("微软雅黑"));
	outtextxy(WIDTH * BLOCKSIZE + BLOCKSIZE, (HEIGHT * BLOCKSIZE) - (BLOCKSIZE * 2), L"重新开始");
	outtextxy(WIDTH * BLOCKSIZE + BLOCKSIZE, (HEIGHT * BLOCKSIZE) - (BLOCKSIZE * 4), L"返回菜单");


	if (x == 2) {
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
				if (x == 1) {
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

				if (x == 2) {
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
	//初始化窗口
	initWindow();
	//初始化开始菜单
	initStartMenu();
	//关闭窗口
	closegraph();

	return 0;
}
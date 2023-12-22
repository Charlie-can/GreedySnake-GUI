#pragma once
#pragma warning(disable:4996)

#include <easyx.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <process.h>


//������ͼ���
#define WIDTH 50
//������ͼ�߶�
#define HEIGHT 40
//��ʼ��Ϸ�ٶ�
#define SPEED 4
//���������С
#define BLOCKSIZE 20
//���ⰴ��ӳ�亯��
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
//���ⰴ��ӳ�䣺��
#define LEFT 37
//���ⰴ��ӳ�䣺��
#define UP 38
//���ⰴ��ӳ�䣺��
#define RIGHT 39
//���ⰴ��ӳ�䣺��
#define DOWN 40
//������Ϸ��ͼ��С��ʼ��
char map[HEIGHT][WIDTH] = { 0 };
//ʳ�����ɱ�־
int fed = 0;
//�ѳ�ʳ������
int fedNum = -1;
// ��Ϸ�ٶȱ���
int speed = SPEED;
//���ƶ��߳��Ƿ������־
bool bSnakeP1Continue = 0, bSnakeP2Continue = 0;
//��P1����
int lengthP1 = 3;
//��P2����
int lengthP2 = 3;
//�Ƿ�����սģʽ
bool isCombat = 0;
//P1��Ҫ�����������
int additionP1 = 0;
//P2��Ҫ�����������
int additionP2 = 0;

//ʱ�����
time_t timep;
//ʱ���
time_t sratTimeStamp;
//������
HANDLE mutex = NULL;
//P1�����P2���
HANDLE gameHandleP1 = NULL, gameHandleP2 = NULL;

//����ṹ��
typedef struct snakeStruct
{
	//X��λ��
	int x;
	//Y��λ��
	int y;
	//��һ����������
	struct snakeStruct* next;

}snake;


//���ýṹ��
typedef struct {
	//ǰʮ
	int tonTen[10];   
	//�Ƿ��Ѷȵ���
	bool difficultyIncreasing;  
	//�Ƿ���ʾ��������
	bool showBackGround;	
	//�ٶ�
	int speed;			
	//��ͼ���
	int width;			
	//��ͼ�߶�
	int height;				
	//�����С
	int blockSize;				

}Setting;

//���߽ṹ��
typedef struct {
	//��������ɫ
	COLORREF color;
	//�ߵ�����ṹ��ָ��
	snake* snakePoints;
}drawSnake;

//P1��ͷȫ�ֱ���
snake* snakeHead;
//P2��ͷȫ�ֱ���
snake* snakeHeadP2;
//���ýṹ��
Setting settings;


/// <summary>
/// ����̰�����趨
/// </summary>
/// <param name="settings">�趨�ṹ��ָ��</param>
void saveSettingsBinary(const Setting* settings);

/// <summary>
/// ��ȡ̰�����趨
/// </summary>
/// <param name="settings">�趨�ṹ��ָ��</param>
void loadSettingsBinary(Setting* settings);


/// <summary>
/// ��ʼ����ͼ��Ԫ��
/// ������ͼ����
/// </summary>
void initMap();

/// <summary>
/// ��ʼ������
/// ���ɺ��ʴ�С
/// </summary>
void initWindow();

/// <summary>
/// ������ͼ��������
/// </summary>
void initLine();

/// <summary>
/// ��ʼ����ͼ
/// �������ڵ�ͼ��λ��
/// </summary>
/// <param name="initP1">�Ƿ�����P1</param>
/// <param name="initP2">�Ƿ�����P2</param>
void initSnake(bool initP1,bool initP2);

/// <summary>
/// ���µ�ͼ�����ڵ�����λ��
/// </summary>
/// <param name="snakePoint">��Ӧ�ߵ���ͷ����</param>
/// <param name="isDelete">
/// �Ƿ�ɾ��
/// ��isDelete=1 ɾ����ͼ�����ڶ�Ӧλ�õ���
/// ��isDelete=0 ������ͼ�����ڶ�Ӧλ�õ���
/// </param>
void updateMap(snake* snakePoint, int isDelete);

/// <summary>
/// ��������
/// </summary>
/// <param name="snakePoint">��Ӧ�ߵ���ͷ����</param>
/// <param name="bodyColor">��Ҫ�滻�������ɫ</param>
void hideSnake(snake* snakePoint, COLORREF bodyColor);

/// <summary>
/// ������
/// ɾ�����һ��������β
/// </summary>
/// <param name="snakePoint">��Ӧ�ߵ���ͷ����</param>
void updateSnake(snake* snakePoint);

/// <summary>
/// ��ʾ��ͷ��λ��
/// </summary>
/// <param name="snakePoint">��Ӧ�ߵ���ͷ����</param>
/// <param name="headColor">��Ҫ������ͷ����ɫ</param>
void showSnake(snake* snakePoint, COLORREF headColor);

/// <summary>
/// �ͷ��ߵ�����
/// </summary>
/// <param name="snakeHead">��Ӧ�ߵ���ͷ����</param>
void freeSnake(snake* snakeHead);

/// <summary>
/// ��Ϸ����
/// </summary>
/// <param name="x">��Ϸ����</param>
void gameOver(int x);

/// <summary>
/// �Ƿ�����
/// �ж����Ƿ�ײǽ��ײ
/// </summary>
/// <param name="snakeP1">�ж���</param>
/// <param name="snakeP2">���ж��� ����ʱ��NULL</param>
/// <returns></returns>
int isDeath(snake* snakeP1, snake* snakeP2);

/// <summary>
/// ����ʳ��
/// </summary>
void food();


/// <summary>
/// ��Ϸ�����
/// </summary>
/// <param name="start">
/// �Ƿ�ʼ
/// 0 δ��ʼ
/// 1 �ѿ�ʼ
/// </param>
/// <param name="multi">
/// �Ƿ����
/// 1 ����
/// 0 ����
/// </param>
void gameBar(bool start, bool multi);


/// <summary>
/// ���ò˵�
/// </summary>
void settingMenu();


/// <summary>
/// ������ڻ����ϵ�����
/// </summary>
/// <param name="snake">�������</param>
void clearSnake(snake* snake);

/// <summary>
/// ������ڻ����ϵ�����
/// </summary>
/// <param name="snake">�������</param>
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

	//��ʼ����ͼΪ' '
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			map[i][j] = ' ';
		}
	}
	
	//������ͼ�߽�
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
	//��ʼ�����ڴ�С
	int x = BLOCKSIZE * WIDTH;
	int y = BLOCKSIZE * HEIGHT;

	initgraph(x + 150, y);

	//  ���ñ���ɫ
	setbkcolor(0xCAE1A4);

	//  ʹ�ñ���ɫ��մ���
	cleardevice();

}

void initLine() {

	int x = BLOCKSIZE * WIDTH;
	int y = BLOCKSIZE * HEIGHT;

	//����������ɫ
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
	//X������



	for (int i = BLOCKSIZE; i < y; i += BLOCKSIZE)
	{
		line(0, i, x, i);

	}
	//Y�����

	return;
}

void initSnake(bool initP1, bool initP2) {
	snake* snakeBody;

	//��ʼ����P2
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

	//��ʼ����P1
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
	//�����ɾ��
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
	//����ͷ��β

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
	//��ʾͷ
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
	//˫�������ж�ͷ��ײ
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
			//���p2Ϊ�٣�֤��������Ϸ��������˫����Ϸ

			return 1;
		}

	}

	if (snakeP2) {
		for (snakePoint = snakeP2; snakePoint; snakePoint = snakePoint->next)
		{
			if ((snakeP1->x == snakePoint->x) && (snakeP1->y == snakePoint->y)) {

				if(isCombat){
				return 2;
				//���Ա���
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

	//������ɵ�ʳ�ﱻ��
	if (!fed) {
		int x, y;

		x = rand() % (WIDTH - 2);
		y = rand() % (HEIGHT - 2);

		//�ж����ɷ�Χ
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
		//���߳���ռ������
		WaitForSingleObject(mutex, INFINITE);


		map[x][y] = '*';
		setfillcolor(YELLOW);
		solidrectangle(((y * BLOCKSIZE) + 1), ((x * BLOCKSIZE) + 1), (((y + 1) * BLOCKSIZE) - 1), (((x + 1) * BLOCKSIZE) - 1));

		fed = 1;
		fedNum++;

		//����
		ReleaseMutex(mutex);

	}
	return;
	//��˫����Ϸ�� �������������
}


void gameBar(bool start, bool multi) {

	//��ʼ������
	settextcolor(0xffffff);

	TCHAR str[BLOCKSIZE];
	LOGFONT fontSetting;
	gettextstyle(&fontSetting);						// ��ȡ��ǰ��������
	fontSetting.lfHeight = BLOCKSIZE+ BLOCKSIZE*0.2;						// ��������߶�
	_tcscpy(fontSetting.lfFaceName, _T("΢���ź�"));		// ��������Ϊ
	fontSetting.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ�����  
	settextstyle(&fontSetting);						// ����������ʽ


	time_t nowTime = time(&timep);
	time_t score = (nowTime - sratTimeStamp) + (fedNum * 10);

	//��Ϸ�ٶȵ���
	if (speed < 6)
		speed = (score / 100) + SPEED;


	//����Ƕ�����Ϸ
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
	//�Ƿ��ǿ�ʼ��Ϸʱ
	if (start) {
		_stprintf(str, _T("�����������ʼ"));
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

	//������ڵ�ͼ�ϵ�λ��

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
	//���ò˵�
	cleardevice();

	ExMessage input;
	input = getmessage(EX_MOUSE | EX_KEY);

	LOGFONT fontSetting;
	gettextstyle(&fontSetting);
	TCHAR str[BLOCKSIZE];
	fontSetting.lfHeight = 48;
	settextstyle(&fontSetting);


	loadSettingsBinary(&settings);
	//��ȡ����



	_stprintf(str, _T("����"));
	outtextxy(((WIDTH * BLOCKSIZE) / 2) + BLOCKSIZE, (BLOCKSIZE)+BLOCKSIZE * 0.2, str);

	fontSetting.lfHeight = 27;
	settextstyle(&fontSetting);

	_stprintf(str, _T("���¶�Ӧ��ĸѡ��ť"));
	outtextxy(WIDTH * BLOCKSIZE - BLOCKSIZE * 3, BLOCKSIZE * 2, str);


	fontSetting.lfHeight = 48;
	settextstyle(&fontSetting);

	_stprintf(str, _T("��ť"));
	outtextxy(BLOCKSIZE * 2, BLOCKSIZE * 4, str);


	while (1)
	{
		input = getmessage(EX_MOUSE | EX_KEY);
		// ��ȡ������Ϣ
		switch (input.message)
		{

		case WM_KEYDOWN:
			if (input.vkcode == VK_ESCAPE)
				return;				// �� ESC ���˳�����
		}


	}
	saveSettingsBinary(&settings);
	//�����ļ�

	return;
}
#include<easyx.h>
#include<conio.h>
#include<graphics.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
#include<mmsystem.h>
#include<dsound.h>
#pragma comment(lib,"Winmm.lib")
const int UNIT_WIDTH = 20;
int type[8][4][4][2] = {
					{{0,0,0,1,1,0,1,1},{0,0,0,1,1,0,1,1},{0,0,0,1,1,0,1,1},{0,0,0,1,1,0,1,1}},
					{{0,0,-1,0,1,0,2,0},{0,0,0,-1,0,1,0,2},{0,0,-1,0,1,0,2,0},{0,0,0,-1,0,1,0,2}},
					{{0,0,0,1,-1,1,1,0},{0,0,-1,0,-1,-1,0,1},{0,0,0,1,-1,1,1,0},{0,0,-1,0,-1,-1,0,1}},
					{{0,0,-1,0,1,0,-1,1},{0,0,0,-1,0,1,-1,-1},{0,0,-1,0,1,0,1,-1},{0,0,0,-1,0,1,1,1}},
					{{0,0,-1,0,1,0,0,1},{0,0,0,-1,0,1,-1,0},{0,0,-1,0,1,0,0,-1},{0,0,1,0,0,-1,0,1}},
					{{0,0,-1,0,0,1,1,1},{0,0,-1,0,-1,1,0,-1},{0,0,-1,0,0,1,1,1},{0,0,-1,0,-1,1,0,-1}},
					{{0,0,-1,0,1,0,1,1},{0,0,0,-1,0,1,-1,1},{0,0,-1,0,1,0,-1,-1},{0,0,0,-1,0,1,1,-1}},
					{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}}
};	//type[��i��ͼ��][��j����ת״̬][��k������][x/y������������ĵ��ƫ����]
int vis[23][15];	//�ö�ά�����¼������Ϸ���棺23��*15�񣩵���ɫ״̬
int color[8] = { 5913579, 3244794, 3258359, 7061280, 11647247, 14325805, 14051128, 13653128 };
int current_shape, next_shape;	//��ǰͼ�Σ���һ��ͼ��
int x, y, rotation;	//��ǰ���ĵ����ꣻ��ǰ��״����ת����
int score, speed;	//��ǰ���֣������Զ������ٶ�
char str[10];	//����ַ��͵Ļ���

void init() {	//��ʼ������ & ѡ���Ѷ�
	srand(time(0) + rand());
	initgraph(700, 500);
	setbkcolor(14735569);
	cleardevice();
	settextcolor(WHITE);
	settextstyle(30, 20, _T("Consolas"));
	outtextxy(150, 220, "Press 1 for Level 1");
	outtextxy(150, 250, "Press 0 for Level ��");
	while (1) {
		char c = _getch();
		if (c != '1' && c != '0')	continue;
		mciSendString("open Music\\CountDown.mp3 alias OP", NULL, 0, NULL);
		mciSendString("play OP", NULL, 0, NULL);
		Sleep(4250);
		if (c == '1') {
			mciSendString("open Music\\bgm1.mp3 alias BGM1", NULL, 0, NULL);
			mciSendString("play BGM1 repeat", NULL, 0, NULL);
			speed = 200;
		}
		else {
			mciSendString("open Music\\bgm0.mp3 alias BGM0", NULL, 0, NULL);
			mciSendString("play BGM0 repeat", NULL, 0, NULL);
			speed = 50;
		}
		break;
	}
	setbkcolor(12759461);
	cleardevice();
	fillrectangle(70, 20, 370, 480);
	fillrectangle(510, 20, 630, 140);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	strcpy_s(f.lfFaceName, _T("Consola"));
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = 900;
	settextstyle(&f);
	settextstyle(25, 15, _T("Consolas"));
	outtextxy(515, 270, "Score:");
	sprintf_s(str, "%d", score);
	outtextxy(615, 270, _T("0"));
	setbkmode(12828338);
}

void Square(int temp_x, int temp_y) {	//�������ĵ����껭��4��С����
	temp_x = 210 + temp_x * UNIT_WIDTH, temp_y = 20 + temp_y * UNIT_WIDTH;
	solidrectangle(temp_x, temp_y, temp_x + UNIT_WIDTH, temp_y + UNIT_WIDTH);
	rectangle(temp_x, temp_y, temp_x + UNIT_WIDTH, temp_y + UNIT_WIDTH);
}

void paint(int temp_color) {
	setfillcolor(temp_color);
	for (int i = 0; i < 4; ++i) {
		if (type[current_shape][rotation][i][1] + y >= 0)	//��ֹԽ���Ͻ�
			Square(type[current_shape][rotation][i][0] + x, type[current_shape][rotation][i][1] + y);
	}
}

void Renew() {
	current_shape = next_shape;	//ʹ����һ����״���µ�ǰ��״
	next_shape = rand() % 8;	//������һ����״
	x = 0, y = 0, rotation = 0;	//�������ĵ����� & ��ת����
	BeginBatchDraw();
	paint(color[current_shape]);
	setfillcolor(WHITE);	//����Ԥ������
	fillrectangle(510, 20, 630, 140);
	setfillcolor(color[next_shape]);	//����ǰ��״��������Ϸ������
	for (int i = 0; i < 4; ++i) {
		int temp_x = 560 + type[next_shape][0][i][0] * UNIT_WIDTH;
		int temp_y = 65 + type[next_shape][0][i][1] * UNIT_WIDTH;
		solidrectangle(temp_x, temp_y, temp_x + UNIT_WIDTH, temp_y + UNIT_WIDTH);
		rectangle(temp_x, temp_y, temp_x + UNIT_WIDTH, temp_y + UNIT_WIDTH);
	}
	FlushBatchDraw();
}

int GameOver() {
	//�����³�����ͼ���У���һС���鸲���˵�ǰ��Ϳɫ���򣨼������ص���������Ϸ����
	for (int i = 0; i < 4; ++i) {
		if (vis[type[current_shape][0][i][1]][type[current_shape][0][i][0] + 7]) {
			Sleep(100);
			BeginBatchDraw();
			settextcolor(BLACK);
			settextstyle(50, 50, _T("Consolas"));
			outtextxy(150, 220, "GAME OVER");
			FlushBatchDraw();
			mciSendString("close BGM1", NULL, 0, NULL);
			mciSendString("close BGM0", NULL, 0, NULL);
			Sleep(100);
			mciSendString("open Music\\ed.mp3 alias ED", NULL, 0, NULL);
			mciSendString("play ED", NULL, 0, NULL);
			return 1;
		}
	}
	return 0;
}

int Check(int tx, int ty, int temp_rotation) {
	for (int i = 0; i < 4; ++i) {
		int temp_x = tx + type[current_shape][temp_rotation][i][0];
		int temp_y = ty + type[current_shape][temp_rotation][i][1];
		//���������ƶ�����ת�󡱵�ͼ�Ρ�Խ�硿�򡾸�����Ϳɫ���򡿣��򲻺Ϸ�������������Ӧ���ƶ�/��ת������
		if (temp_x < -7 || temp_x > 7 || temp_y > 22 || vis[temp_y][temp_x + 7])	return 0;
	}
	return 1;
}

int Stop() {
	for (int i = 0; i < 4; ++i) {
		int temp_x = x + type[current_shape][rotation][i][0], temp_y = y + type[current_shape][rotation][i][1];
		//������ǰͼ����һ�㼴���桿�򡾵�ǰͼ������һС�����·�һ��Ϊ��Ϳɫ���򡿣��򷵻�1����ֹ��ǰͼ�ε��жϲ�������һ��ͼ��
		if (temp_y + 1 > 22 || vis[temp_y + 1][temp_x + 7])	return 1;
	}
	return 0;
}

void CheckLine() {	//�����ж�
	for (int i = 22; i >= 0; --i) {
		int count = 0;
		for (int j = 0; j <= 14; ++j)
			count += vis[i][j] ? 1 : 0;
		if (count == 15) {	//��������15����Ϳɫ��������Ҫ���� �� ʹ����һ�е���ɫ���Ǹ�����ɫ
			for (int k = i; k >= 0; --k)
				for (int j = 0; j <= 14; ++j)
					vis[k][j] = (k == 0) ? 0 : vis[k - 1][j];
			++i;	//�Ը��º�ĵ�ǰ�л������һ���ж�
			BeginBatchDraw();
			//���»���ȫ��
			for (int k = 0; k <= 22; ++k) {
				for (int j = 0; j <= 14; ++j) {
					setfillcolor(!vis[k][j] ? WHITE : vis[k][j]);
					solidrectangle(70 + j * UNIT_WIDTH, 20 + k * UNIT_WIDTH, 90 + j * UNIT_WIDTH, 40 + k * UNIT_WIDTH);
					rectangle(70 + j * UNIT_WIDTH, 20 + k * UNIT_WIDTH, 90 + j * UNIT_WIDTH, 40 + k * UNIT_WIDTH);
				}
			}
			//���¼Ʒְ�
			setfillcolor(12759461);
			solidrectangle(605, 270, 650, 340);
			sprintf_s(str, "%d", ++score);
			outtextxy(610, 270, str);
			Sleep(200);
			FlushBatchDraw();
			mciSendString("close BGM2", NULL, 0, NULL);
			mciSendString("open Music\\Disappear.mp3 alias BGM2", NULL, 0, NULL);
			mciSendString("play BGM2", NULL, 0, NULL);
			if (speed >= 0)	speed -= 10;	//ÿ����һ�У������Զ������ٶ����ӣ���just for fun��
		}
	}
}

void Play() {
	next_shape = rand() % 8;	//���������һ��ͼ��
	while (1) {
		Renew();	//������Ϸ����
		if (GameOver()) 	return;	//����Ϸ�������򷵻�������
		DWORD time1 = GetTickCount();
		DWORD time3 = GetTickCount();
		while (1) {
			DWORD time2 = GetTickCount();
			if (time2 - time1 >= speed) {	//��speed��Ƶ�ʣ�ʹ��ǰ�����Զ�����
				if (!Stop()) {	//���޷��������䣬��ʹ�õ�ǰ�������ɫ���ԡ���ǰ����ռ���������Ϳɫ��
					time1 = time2;
					//��ǰͼ���Զ�����һ��  
					BeginBatchDraw();
					paint(WHITE);	//�ٲ�����ǰͼ��
					++y;	//�ڸ���������
					paint(color[current_shape]);	//��ʹ�ø��º��������Ƶ�ǰͼ��
					FlushBatchDraw();
				}
			}
			if (_kbhit()) {	//��ȡ���̲���
				char val = _getch();
				BeginBatchDraw();
				paint(WHITE);	//������ǰͼ��
				//�жϳ��Ը��µġ�x���꡿/��y���꡿/����ת�����Ƿ�Ϸ� �� ���Ϸ�����и���
				if (val == ' ')	system("pause");	//���׼�
				if (val == 'q')	current_shape = 0;	//���׼�
				if (val == 'e')	current_shape = 1;	//���׼�
				if (val == 's' && Check(x, y + 1, rotation))	++y;
				if (val == 'a' && Check(x - 1, y, rotation))	--x;
				if (val == 'd' && Check(x + 1, y, rotation))	++x;
				if (val == 'k' && Check(x, y, (rotation + 1) % 4))	rotation = (rotation + 1) % 4;
				paint(color[current_shape]);	//ʹ�á�ԭ״̬��/�����º�����꡿/����ת���򡿻��Ƶ�ǰͼ��
				FlushBatchDraw();
			}
			DWORD time4 = GetTickCount();
			if (time4 - time3 >= 1000) {
				time3 = time4;
				if (Stop()) {
					for (int i = 0; i < 4; ++i)
						vis[y + type[current_shape][rotation][i][1]][x + type[current_shape][rotation][i][0] + 7] = color[current_shape];
					CheckLine();	//����
					break;
				}
			}
		}
	}
}

int main() {
	init();
	Play();
	system("Pause");
}
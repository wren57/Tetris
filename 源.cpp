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
};	//type[第i种图形][第j种旋转状态][第k个格子][x/y坐标相对于中心点的偏移量]
int vis[23][15];	//用二维数组记录区域（游戏界面：23格*15格）的颜色状态
int color[8] = { 5913579, 3244794, 3258359, 7061280, 11647247, 14325805, 14051128, 13653128 };
int current_shape, next_shape;	//当前图形；下一个图形
int x, y, rotation;	//当前中心点坐标；当前形状的旋转方向
int score, speed;	//当前积分；方块自动下落速度
char str[10];	//存放字符型的积分

void init() {	//初始化界面 & 选择难度
	srand(time(0) + rand());
	initgraph(700, 500);
	setbkcolor(14735569);
	cleardevice();
	settextcolor(WHITE);
	settextstyle(30, 20, _T("Consolas"));
	outtextxy(150, 220, "Press 1 for Level 1");
	outtextxy(150, 250, "Press 0 for Level ∞");
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

void Square(int temp_x, int temp_y) {	//根据中心点坐标画出4个小方块
	temp_x = 210 + temp_x * UNIT_WIDTH, temp_y = 20 + temp_y * UNIT_WIDTH;
	solidrectangle(temp_x, temp_y, temp_x + UNIT_WIDTH, temp_y + UNIT_WIDTH);
	rectangle(temp_x, temp_y, temp_x + UNIT_WIDTH, temp_y + UNIT_WIDTH);
}

void paint(int temp_color) {
	setfillcolor(temp_color);
	for (int i = 0; i < 4; ++i) {
		if (type[current_shape][rotation][i][1] + y >= 0)	//防止越过上界
			Square(type[current_shape][rotation][i][0] + x, type[current_shape][rotation][i][1] + y);
	}
}

void Renew() {
	current_shape = next_shape;	//使用下一个形状更新当前形状
	next_shape = rand() % 8;	//更新下一个形状
	x = 0, y = 0, rotation = 0;	//重置中心点坐标 & 旋转方向
	BeginBatchDraw();
	paint(color[current_shape]);
	setfillcolor(WHITE);	//更新预览窗口
	fillrectangle(510, 20, 630, 140);
	setfillcolor(color[next_shape]);	//将当前形状绘制在游戏界面上
	for (int i = 0; i < 4; ++i) {
		int temp_x = 560 + type[next_shape][0][i][0] * UNIT_WIDTH;
		int temp_y = 65 + type[next_shape][0][i][1] * UNIT_WIDTH;
		solidrectangle(temp_x, temp_y, temp_x + UNIT_WIDTH, temp_y + UNIT_WIDTH);
		rectangle(temp_x, temp_y, temp_x + UNIT_WIDTH, temp_y + UNIT_WIDTH);
	}
	FlushBatchDraw();
}

int GameOver() {
	//若更新出来的图形中，任一小方块覆盖了当前已涂色区域（即出现重叠），则游戏结束
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
		//若“尝试移动或旋转后”的图形【越界】或【覆盖已涂色区域】，则不合法（即不进行相应的移动/旋转操作）
		if (temp_x < -7 || temp_x > 7 || temp_y > 22 || vis[temp_y][temp_x + 7])	return 0;
	}
	return 1;
}

int Stop() {
	for (int i = 0; i < 4; ++i) {
		int temp_x = x + type[current_shape][rotation][i][0], temp_y = y + type[current_shape][rotation][i][1];
		//若【当前图形下一层即地面】或【当前图形中任一小方块下方一格为已涂色区域】，则返回1：终止当前图形的判断并更新下一个图形
		if (temp_y + 1 > 22 || vis[temp_y + 1][temp_x + 7])	return 1;
	}
	return 0;
}

void CheckLine() {	//消行判断
	for (int i = 22; i >= 0; --i) {
		int count = 0;
		for (int j = 0; j <= 14; ++j)
			count += vis[i][j] ? 1 : 0;
		if (count == 15) {	//若本行有15个已涂色区域，则需要消行 → 使用上一行的颜色覆盖该行颜色
			for (int k = i; k >= 0; --k)
				for (int j = 0; j <= 14; ++j)
					vis[k][j] = (k == 0) ? 0 : vis[k - 1][j];
			++i;	//对更新后的当前行还需进行一次判断
			BeginBatchDraw();
			//重新绘制全局
			for (int k = 0; k <= 22; ++k) {
				for (int j = 0; j <= 14; ++j) {
					setfillcolor(!vis[k][j] ? WHITE : vis[k][j]);
					solidrectangle(70 + j * UNIT_WIDTH, 20 + k * UNIT_WIDTH, 90 + j * UNIT_WIDTH, 40 + k * UNIT_WIDTH);
					rectangle(70 + j * UNIT_WIDTH, 20 + k * UNIT_WIDTH, 90 + j * UNIT_WIDTH, 40 + k * UNIT_WIDTH);
				}
			}
			//更新计分板
			setfillcolor(12759461);
			solidrectangle(605, 270, 650, 340);
			sprintf_s(str, "%d", ++score);
			outtextxy(610, 270, str);
			Sleep(200);
			FlushBatchDraw();
			mciSendString("close BGM2", NULL, 0, NULL);
			mciSendString("open Music\\Disappear.mp3 alias BGM2", NULL, 0, NULL);
			mciSendString("play BGM2", NULL, 0, NULL);
			if (speed >= 0)	speed -= 10;	//每消除一行，方块自动下落速度增加（※just for fun）
		}
	}
}

void Play() {
	next_shape = rand() % 8;	//随机生成下一个图形
	while (1) {
		Renew();	//更新游戏界面
		if (GameOver()) 	return;	//若游戏结束，则返回主函数
		DWORD time1 = GetTickCount();
		DWORD time3 = GetTickCount();
		while (1) {
			DWORD time2 = GetTickCount();
			if (time2 - time1 >= speed) {	//以speed的频率，使当前方块自动下落
				if (!Stop()) {	//若无法继续下落，则【使用当前方块的颜色】对【当前方块占用区域进行涂色】
					time1 = time2;
					//当前图形自动下落一行  
					BeginBatchDraw();
					paint(WHITE);	//①擦除当前图形
					++y;	//②更新纵坐标
					paint(color[current_shape]);	//③使用更新后的坐标绘制当前图形
					FlushBatchDraw();
				}
			}
			if (_kbhit()) {	//读取键盘操作
				char val = _getch();
				BeginBatchDraw();
				paint(WHITE);	//擦除当前图形
				//判断尝试更新的【x坐标】/【y坐标】/【旋转方向】是否合法 → 若合法则进行更新
				if (val == ' ')	system("pause");	//作弊键
				if (val == 'q')	current_shape = 0;	//作弊键
				if (val == 'e')	current_shape = 1;	//作弊键
				if (val == 's' && Check(x, y + 1, rotation))	++y;
				if (val == 'a' && Check(x - 1, y, rotation))	--x;
				if (val == 'd' && Check(x + 1, y, rotation))	++x;
				if (val == 'k' && Check(x, y, (rotation + 1) % 4))	rotation = (rotation + 1) % 4;
				paint(color[current_shape]);	//使用【原状态】/【更新后的坐标】/【旋转方向】绘制当前图形
				FlushBatchDraw();
			}
			DWORD time4 = GetTickCount();
			if (time4 - time3 >= 1000) {
				time3 = time4;
				if (Stop()) {
					for (int i = 0; i < 4; ++i)
						vis[y + type[current_shape][rotation][i][1]][x + type[current_shape][rotation][i][0] + 7] = color[current_shape];
					CheckLine();	//消行
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
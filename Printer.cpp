// test1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <graphics.h>
#include <time.h>
#include <conio.h>
#include "SnakeDot.h"


#define ROW 46
#define COL 64

typedef struct _MYCOORD {
	SHORT X;
	SHORT Y;
	size_t color;
} MYCOORD, *PMYCOORD;
//枚举
enum game
{
	SPACE, WALL, SNAKE, FOOD, HEAD//, TRAIL//空地  墙  蛇  食物 (轨迹）
};
/************全局变量************/
int step;//走了多少步 有多少trail
DWORD t1, t2;			//定义两个时间控制移动速度
int map[ROW][COL];    //地图大小

MYCOORD trail[1024];

COORD snake[1024];
char SnakeDir;			//蛇移动方向
size_t SnakeSize;
size_t SnakeColor;
size_t PrevSnakeColor;
void DrawMap();	//画图

void init();	//初始化游戏数据
void ChangeDir();	//修改蛇的方向
void move();//蛇的移动
// 主函数
int main()
{
	initgraph(640, 480);
	init();               //初始化
	while (true)
	{
		t2 = GetTickCount();
		DrawMap();
	
		if (_kbhit())
		{
			ChangeDir();
			move();
			t2 = GetTickCount();
			t1 = t2;
		}
		if (t2 - t1 > 50)
		{
			move();
			t1 = t2;
		}
	}

	getchar();
	closegraph();          // 关闭绘图窗口
	return 0;
}

void init() {
	srand((unsigned)time(NULL));	//随机种子
	setbkcolor(WHITE);				//设置背景颜色

	memset(map, SPACE, sizeof(map));//初始化map数组为0即SPACE
	
	//第一列和最后一列为墙							
	for (int i = 0; i < ROW; i++)
	{
		map[i][0] = map[i][COL - 1] = WALL;
	}
	//第一行和最后一行为墙	
	for (int j = 1; j < COL - 1; j++)
	{
		map[0][j] = map[ROW - 1][j] = WALL;
	}

	//定义蛇头和蛇的身体
	map[3][5] = HEAD;

	//初始化蛇
	SnakeSize = 3;  //蛇 长

	snake[0].X = 3;
	snake[0].Y = 5;

	trail[0].X = 3;
	trail[0].Y = 5;
	step = 0;

	SnakeDir = 'D';	//蛇方向向右
	SnakeColor = RED;
}

void DrawMap() {
	BeginBatchDraw();  //开始绘图
	setbkcolor(WHITE);  //设置背景颜色为白色
	settextcolor(RGB(238, 0, 0));
	cleardevice();    //清屏

	for (int y = 0; y < ROW; y++)    //y轴方向向下
	{
		for (int x = 0; x < COL; x++)  //x轴方向下上
		{
			switch (map[y][x])
			{
			case SPACE:
				break;
			case WALL:
				setlinecolor(BLACK);
				setfillcolor(RGB(238, 233, 233));  //灰色
				fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
				break;
			case HEAD:
				//画蛇头
				switch (rand() % 3)
				{
				case 0:
					setfillcolor(RED);    //红色 255 0 0
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					SnakeColor = RED;
					break;
				case 1:
					setfillcolor(YELLOW);    //橙  255 165 0 
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					SnakeColor = YELLOW;
					break;
				case 2:
					setfillcolor(BLUE);    //黄  255 255 0
					solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
					SnakeColor = BLUE;
					break;

				default:
					break;
				}
				break;
			//case TRAIL:
			//	setfillcolor(PrevSnakeColor);   //显示之前头的颜色
			//	solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
			//	break;
			}	
		}
	}
	for (int k = 0; k < step; k++) {
		setfillcolor(trail[k].color);
		solidrectangle(trail[k].X * 10, trail[k].Y * 10 + 20, trail[k].X * 10 + 10, trail[k].Y * 10 + 30);
	}
	EndBatchDraw();
}

void ChangeDir()
{
	switch (_getch())
	{
	case'A':
	case'a':
	case 75:
		SnakeDir = 'A';	

		break;
	case'D':
	case'd':
	case 77:
		SnakeDir = 'D';
		break;
	case'W':
	case'w':
	case 72:
		SnakeDir = 'W';
		break;
	case'S':
	case's':
	case 80:
		SnakeDir = 'S';
		break;
	case 32:
		_getch();
		break;
	default:
		break;
	}
}

void move()
{
	COORD next;		//蛇头的下一个位置
	switch (SnakeDir)
	{
	case'A':
		next.X = snake[0].X;
		next.Y = snake[0].Y - 1;
		break;
	case'W':
		next.X = snake[0].X - 1;
		next.Y = snake[0].Y;
		break;
	case'D':
		next.X = snake[0].X;
		next.Y = snake[0].Y + 1;
		break;
	case'S':
		next.X = snake[0].X + 1;
		next.Y = snake[0].Y;
		break;
	default:
		break;
	}
	switch (map[next.X][next.Y])
	{
	case SPACE://直接移动
		//每move一次k+1，trail保存当前头的位置
			trail[step].color = SnakeColor;
			//PrevSnakeColor = SnakeColor;
			trail[step].Y = snake[0].X;
			trail[step].X = snake[0].Y;
			map[snake[0].X][snake[0].Y] = SPACE;
			snake[0] = next;											//将下一个位置赋值给蛇头
			map[snake[0].X][snake[0].Y] = HEAD;							//设置头
			step++;
		break;
	case WALL:
		break;
	case SNAKE:
		break;
	case FOOD://食物        蛇尾不变
		break;
	default:break;
	}
}

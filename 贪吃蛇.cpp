// 贪吃蛇.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>

#define SNAKE_LENGTH 100//定义蛇的最大长度
#define SCREEN_WIDETH 80
#define SCREEN_HEIGHT 30

//枚举方向
enum KEY {
	right = VK_RIGHT,
	left = VK_LEFT,
	up = VK_UP,
	down = VK_DOWN
};
//定义每一节蛇的坐标
struct coor {
	int x;
	int y;
};
//定义蛇的属性
struct  snake {
	int len;//当前蛇的长度
	struct coor coord[SNAKE_LENGTH];//每一节蛇的坐标
	int key;//定义蛇的方向
	int speed;
	int flag;//定义蛇的状态   1表示存活   0表示死亡
}snake;

//光标移动函数
void gotoxy(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//食物的信息
struct foodcoord {
	int x;
	int y;
	int flag;//定义食物的状态
}food;

//蛇的移动
void move_snake();
//画出蛇
void draw_snake();
//产生食物
void creatfood();
//判断蛇是否吃到食物
void eatfood();
//判断蛇是否死掉
void SnakeState();
//初始化游戏界面
void init_sence()
{
	//初始化上下墙
	for (int i = 0; i < SCREEN_WIDETH; i += 2)
	{
		gotoxy(i, 0);
		printf("■");
		gotoxy(i, SCREEN_HEIGHT);
		printf("■");
	}
	//初始化左右墙
	for (int i = 0; i <= SCREEN_HEIGHT; i++)
	{
		gotoxy(0, i);
		printf("■");
		gotoxy(SCREEN_WIDETH, i);
		printf("■");
	}
	//打印提示信息
	gotoxy(SCREEN_WIDETH + 5, 2);
	printf("\t贪吃蛇");
	gotoxy(SCREEN_WIDETH + 5, 4);
	printf("\t2020-01-07");
	gotoxy(SCREEN_WIDETH + 5, 8);
	gotoxy(SCREEN_WIDETH + 5, 10);
	printf("\tF1:加速\n");
	gotoxy(SCREEN_WIDETH + 5, 12);
	printf("\tF2：减速\n");
	gotoxy(SCREEN_WIDETH + 5, 14);
	printf("\tCTRL:继续\n");
	gotoxy(SCREEN_WIDETH + 5, 16);
	printf("\t空格：暂停\n");
	gotoxy(SCREEN_WIDETH + 5, 18);
	printf("\tESC:退出游戏");
	gotoxy(SCREEN_WIDETH + 5, 28);
}

int main()
{
	//设置窗口大小
	system("mode con cols=110 lines=31");
	//设置标题
	SetConsoleTitleA("贪吃蛇");

	//初始化蛇
	while (true) {
		snake.key = VK_RIGHT;//初始化方向
		snake.len = 5; //初始化长度
		snake.speed = 300;//初始化蛇的移动速度
		snake.coord[1].x = SCREEN_WIDETH / 2;    //初始化蛇头的坐标
		snake.coord[1].y = SCREEN_HEIGHT / 2;
		snake.coord[2].x = SCREEN_WIDETH / 2 - 2;//初始化蛇头的坐标
		snake.coord[2].y = SCREEN_HEIGHT / 2;
		snake.coord[3].x = SCREEN_WIDETH / 2 - 4;//初始化蛇头的坐标
		snake.coord[3].y = SCREEN_HEIGHT / 2;

		//初始化食物状态
		food.flag = 1;//1表示吃到食物   0表示没有吃到食物
		//初始化蛇的状态
		snake.flag = 1;//1活  0死

		init_sence();//初始化游戏界面
		while (1)
		{
			draw_snake();//画蛇
			Sleep(snake.speed);//蛇的移动速度
			move_snake();//移动蛇
			if (food.flag)
				creatfood();//产生食物
			eatfood();//判断是否吃到食物
			SnakeState();//判断蛇是否死亡
			if (!snake.flag)break;
		}
		system("cls");
		gotoxy(SCREEN_WIDETH / 2, SCREEN_HEIGHT / 2 - 4);
		printf("\t游戏结束!!!");
		gotoxy(SCREEN_WIDETH / 2 - 6, SCREEN_HEIGHT / 2 + 2);
		printf("你的得分是：\t\t\t%d  ", snake.len - 1);
		gotoxy(SCREEN_WIDETH / 2 - 6, SCREEN_HEIGHT / 2 + 4);
		printf("重新开始：\t\t\tCTRL ");
		gotoxy(SCREEN_WIDETH / 2 - 6, SCREEN_HEIGHT / 2 + 6);
		printf("退出游戏:\t\t\tESC");

		while (1)
		{
			if (GetAsyncKeyState(VK_CONTROL))
			{
				system("cls");
				break;
			}
			else if (GetAsyncKeyState(VK_ESCAPE))
				return 0;
		}
	}	
}

//蛇的移动
void move_snake()
{
	//判断是否有按键操作
	if (GetAsyncKeyState(up))
	{
		if (snake.key != down)snake.key = up;
	}
	else if (GetAsyncKeyState(down))
	{
		if (snake.key != up)snake.key = down;
	}
	else if (GetAsyncKeyState(right))
	{
		if (snake.key != left)snake.key = right;
	}
	else if (GetAsyncKeyState(left))
	{
		if (snake.key != right)snake.key = left;
	}
	else if (GetAsyncKeyState(VK_F1))
	{
		if (snake.speed >= 100)snake.speed -= 50;
	}
	else if (GetAsyncKeyState(VK_F2))
	{
		if (snake.speed <= 3000)snake.speed += 100;
	}
	else if (GetAsyncKeyState(VK_SPACE)) {
		while (1) {
			if (GetAsyncKeyState(VK_CONTROL))
				break;
		}
	}
	else if (GetAsyncKeyState(VK_ESCAPE))
		return ;
	//根据检测到的方向改变蛇头的位置
	switch (snake.key)
	{
		case right:snake.coord[1].x += 2; break;
		case left:snake.coord[1].x -= 2; break;
		case up:snake.coord[1].y -= 1; break;
		case down:snake.coord[1].y += 1; break;
	}
}

//画出蛇
void draw_snake()
{
	//画出蛇头
	gotoxy(snake.coord[1].x, snake.coord[1].y);
	printf("□");

	//画出蛇身，直接一个for循环实现
	for (int i = 2; i <= snake.len; i++)
	{
		gotoxy(snake.coord[i].x, snake.coord[i].y);
		printf("□");
	}
	//擦掉尾巴
	gotoxy(snake.coord[snake.len].x, snake.coord[snake.len].y);
	printf("  ");

	//遍历每一节蛇
	for (int i = snake.len; i > 1; i--)
	{
		snake.coord[i].x = snake.coord[i - 1].x;
		snake.coord[i].y = snake.coord[i - 1].y;
	}

	gotoxy(0, 0);
	printf("■");
	gotoxy(85, 25);
	printf("得分:%d  ", snake.len - 1);

}

//产生食物
void creatfood()
{
	//随机种子生成
	srand((unsigned)time(NULL));
	if (food.flag)
		while (1)
		{
			food.x = rand() % 80;
			food.y = rand() % 30;
			if (food.x % 2 == 0 && food.x >= 2 && food.x <= 78 && food.y > 1 && food.y < 30)
			{
				int flag = 0;
				//判断产生的食物可不可能在蛇的身体上
				for (int i = 1; i <= snake.len; i++)
				{
					if (snake.coord[i].x == food.x&&snake.coord[i].y == food.y)
					{
						flag = 1;
						break;
					}
				}
				if (flag)continue;
				//绘制食物
				else
				{
					gotoxy(food.x, food.y);
					printf("⊙");
					food.flag = 0;
					break;
				}
			}
		}
	food.flag = 0;
}

//判断蛇是否吃到食物
void eatfood()
{
	//只需要判断蛇头是否与食物重合
	if (food.x == snake.coord[1].x&&food.y == snake.coord[1].y)
	{
		snake.len += 1;
		food.flag = 1;
	}
}

//判断蛇是否死掉
void SnakeState()
{
	if (snake.coord[1].x < 2 || snake.coord[1].x>78 || snake.coord[1].y < 1 || snake.coord[1].y>29)
		snake.flag = 0;

	for (int i = 2; i <= snake.len; i++)
	{
		if (snake.coord[1].x == snake.coord[i].x&&snake.coord[1].y == snake.coord[i].y)
			snake.flag = 0;
	}
}
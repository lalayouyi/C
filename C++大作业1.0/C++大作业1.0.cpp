//******************************************************头文件导入部分*************************************************************
#include<iostream>
#include<stdlib.h>
#include<graphics.h>//图形库
#include<conio.h>//控制输出的头文件，_getch应用
#include<windows.h>
#include<time.h>
#include<string>
#include<math.h>
#include <fstream>
#pragma comment(lib,"winmm.lib") //背景音乐
using namespace std;
//******************************************************五子棋部分*****************************************************************

//------------------------------------------------------所有用到的函数定义以及功能说明---------------------------------------------
void initgame();//可视化页面初始化
void renjibegingame();//游戏开始，相关内容加载
void gameover();//游戏结束
int panduan(COLORREF, int, int);//判断是否五子连棋
void rjarrayqizhi();//机器下棋
void rrarrayqizhi();//人下棋
int shpanduan(COLORREF, int, int, int, int);//巧妙借助颜色判断机器落子
int scorepanduan(int, int);//人机每一步分析可用棋子上的分数值,返回分数最高的就是落子点  

//----------------------------------------------------相关量定义-----------------------------------------------------------------
typedef struct
{
	int x;
	int y;
}post;//人鼠标

typedef struct
{
	int x;
	int y;
	int score;
}alph;//机器

post xy;//光标设置

int color = 0;//黑白判断

//存储人下的位置
post huixy;

//存储机器下的位置
alph rjhuixy;

//定义一个稍微大一点的结构体数组来存放每一步棋子顺序(人)
post rrarray[1000] = { 0 };

//定义一个稍微大一点的结构体数组来存放每一步棋子顺序(机)
alph rjarray[1000] = { 0 };

//--------------------------------------------------------函数功能实现----------------------------------------------------------------

void initgame()
{
	//设置窗口大小
	initgraph(500, 750);


	// 获得窗口句柄
	HWND hWnd = GetHWnd();
	// 使用 API 函数修改窗口名称
	SetWindowText(hWnd, "欢迎体验右一杂货铺的欢乐五子棋");


	//加载图片
	IMAGE backgrand;
	loadimage(&backgrand, "BGM.png", 500, 750);
	putimage(0, 0, &backgrand);


	//设置背景颜色
	setbkcolor(RGB(176, 183, 182));    //目的是为了让显示的文字看起来没有边框
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, "楷体");
	outtextxy(30, 10, "你是黑子，你先来");
	outtextxy(30, 50, "祝玩得开心！O(∩_∩)O");

}

void renjibegingame()
{
	//加载背景音乐
	mciSendString("open BGM.mp3 alias bg", NULL, 0, NULL);
	mciSendString("play bg repeat", NULL, 0, NULL);//循环播放
A:

	// 棋盘加载
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID);
	setfillcolor(RGB(187, 161, 139));
	fillrectangle(25, 275, 425, 675); //计算而来的
	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 15; j++)
		{
			rectangle(25 + j * 25, 275 + i * 25, 50 + j * 25, 300 + i * 25);  //棋盘左上是50 300

		}
	}

	//黑子先手  游戏开始光标默认在正中间
	//绘制光标
	xy.x = 225;
	xy.y = 475;     // 这个坐标是推出来的 就是最中心的坐标
	setlinecolor(RED);
	setlinestyle(PS_DOT);
	rectangle(xy.x - 15, xy.y - 15, xy.x + 15, xy.y + 15);

	//初始化人机阿尔法猫
	//加载每个点并且把分数搞成0 
	alph alphcat[225];
	//填充电脑位置     //棋盘左上是50 300
	int inde = 0; //定义结构体数组的下标
	for (int i = 1; i <= 15; i++)
	{
		for (int j = 1; j <= 15; j++)
		{
			//25 + j * 25, 275 + i * 25,  //棋盘左上是50 300
			alphcat[inde].x = 25 + j * 25;
			alphcat[inde].y = 275 + i * 25;
			alphcat[inde].score = 0;
			inde++;
		}
	}

	//定义一个有225个元素的结构体数组来代表光标的所有分布的可能性 
	post master[225];
	//填充电脑位置     //棋盘左上是50 300
	int index = 0; //定义结构体数组的下标
	for (int i = 1; i <= 15; i++)
	{
		for (int j = 1; j <= 15; j++)
		{
			//25 + j * 25, 275 + i * 25,  //棋盘左上是50 300
			master[index].x = 25 + j * 25;
			master[index].y = 275 + i * 25;
			index++;
		}
	}

	//清除上一下鼠标缓存
	FlushMouseMsgBuffer();

	MOUSEMSG m;		// 定义鼠标消息

	while (true)
	{


		//获取一条鼠标消息
		m = GetMouseMsg();



		//显示提示黑子先手或者显示提示当前将要下的棋子颜色
		if (color == 0)
		{
			setbkcolor(RGB(176, 183, 182));
			settextcolor(RGB(0, 0, 0));
			settextstyle(20, 0, "楷体");
			outtextxy(30, 250, "黑子先手!");
		}
		else if (color % 2 != 0)
		{
			setbkcolor(RGB(176, 183, 182));
			settextcolor(RGB(0, 0, 0));
			settextstyle(20, 0, "楷体");
			outtextxy(30, 250, "轮到棋子:");
			//提示落子颜色
			setfillcolor(WHITE);
			setlinecolor(WHITE);
			fillcircle(130, 260, 10);
		}
		else
		{
			setbkcolor(RGB(176, 183, 182));
			settextcolor(RGB(0, 0, 0));
			settextstyle(20, 0, "楷体");
			outtextxy(30, 250, "轮到棋子:");
			//提示落子颜色
			setfillcolor(BLACK);
			setlinecolor(BLACK);
			fillcircle(130, 260, 10);
		}
		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:
			for (int i = 0; i < 225; i++)
			{
				if ((m.x >= master[i].x - 10 && m.x <= master[i].x + 10) && (m.y >= master[i].y - 10 && m.y <= master[i].y + 10))
				{
					setlinecolor(RGB(187, 161, 139));
					rectangle(xy.x - 15, xy.y - 15, xy.x + 15, xy.y + 15);
					xy.x = master[i].x;
					xy.y = master[i].y;
					setlinecolor(RED);
					setlinestyle(PS_DOT);
					rectangle(xy.x - 15, xy.y - 15, xy.x + 15, xy.y + 15);
				}
			}
			break;

		case WM_LBUTTONUP:
		{
			//不可以重复下棋  通过getpixel这个函数来实现
			if (getpixel(xy.x - 3, xy.y - 3) == RGB(0, 0, 0) || getpixel(xy.x - 3, xy.y - 3) == RGB(255, 255, 255))
			{
				HWND hWnd = GetHWnd();
				MessageBox(hWnd, "这里已经有棋子了请重新选择", "提示", MB_OK);
				break;
			}
			//把下的棋的顺序保存在array数组里面
			rrarray[color] = xy;

			setfillcolor(BLACK);
			setlinecolor(BLACK);
			fillcircle(xy.x, xy.y, 10);

			COLORREF hei = getpixel(xy.x - 3, xy.y - 3);
			//胜负判断
			int a = panduan(hei, xy.x, xy.y);
			if (a == 1)
			{
				setbkcolor(RGB(176, 183, 182));
				settextcolor(RGB(0, 0, 0));
				settextstyle(30, 0, "宋体");
				outtextxy(105, 150, "好耶，你赢啦，你真厉害！");
				outtextxy(65, 120, "(请按任意键继续游戏)");
				char str[30];
				sprintf_s(str, "大战了%d回合!", color / 2 + 1);
				outtextxy(105, 190, str);


				//显示棋子顺序
				rrarrayqizhi();
				rjarrayqizhi();

				//重置color
				color = 0;
				system("pause");
				goto A;
			}
			//加入平局的判断
			if (a == 0 && color == 224)   //因为color最大值是224
			{
				setbkcolor(RGB(176, 183, 182));
				settextcolor(RGB(0, 0, 0));
				settextstyle(30, 0, "宋体");
				outtextxy(80, 150, "我和你不相上下嘛!");
				outtextxy(65, 120, "(请按任意键继续游戏)");


				//显示棋子顺序
				rrarrayqizhi();
				rjarrayqizhi();

				//重置color
				color = 0;
				system("pause");
				goto A;
			}

			//把当前下的棋保存在huixy中
			huixy.x = xy.x;
			huixy.y = xy.y;
			color++;
			Sleep(100);
		}
		case 1:
		{
			for (int i = 0; i < 225; i++)
			{
				if (RGB(0, 0, 0) != getpixel(alphcat[i].x - 3, alphcat[i].y - 3) && RGB(255, 255, 255) != getpixel(alphcat[i].x - 3, alphcat[i].y - 3)/*当前落棋点没有落棋 (这个点的颜色是背景色)*/)
				{
					//RGB(255, 255, 255) != getpixel(alphcat[i].x - 3, alphcat[i].y - 3)
					alphcat[i].score = scorepanduan(alphcat[i].x, alphcat[i].y);
				}
			}
			//再来选出一个得分最大的,如果得分一样就随机从最大的一个选一个  
			alph alphmax;
			alphmax.score = alphcat[0].score;
			for (int i = 1; i < 225; i++)
			{
				if (alphcat[i].score >= alphmax.score)
				{
					alphmax = alphcat[i];
				}
			}

			// 把下的棋的顺序保存在array数组里面
			rjarray[color] = alphmax;

			//根据这个点画白棋
			setfillcolor(WHITE);
			setlinecolor(WHITE);
			fillcircle(alphmax.x, alphmax.y, 10);

			for (int i = 0; i < 225; i++)
			{
				alphcat[i].score = 0;
			}
			COLORREF bai = getpixel(alphmax.x - 3, alphmax.y - 3);
			//胜负判断
			int b = panduan(bai, alphmax.x, alphmax.y);
			if (b == 1)
			{
				settextcolor(RGB(0, 0, 0));
				settextstyle(30, 0, "宋体");
				outtextxy(105, 150, "承让啦，还是我厉害一点啦");
				outtextxy(65, 120, "(请按任意键继续游戏)");
				char str[30];
				sprintf_s(str, "大战了%d回合!", color / 2 + 1);
				outtextxy(105, 190, str);


				//显示棋子顺序
				rrarrayqizhi();
				rjarrayqizhi();

				//重置color
				color = 0;
				system("pause");
				goto A;
			}
			//加入平局的判断
			if (b == 0 && color == 224)   //因为color最大值是224
			{
				settextcolor(RGB(0, 0, 0));
				settextstyle(30, 0, "宋体");
				outtextxy(250, 150, "我和你五五开!");
				outtextxy(65, 120, "(请按任意键继续游戏)");


				//显示棋子顺序
				rrarrayqizhi();
				rjarrayqizhi();

				//重置color
				color = 0;
				system("pause");
				goto A;
			}
			//把当前下的棋保存在rjhuixy中
			rjhuixy.x = alphmax.x;
			rjhuixy.y = alphmax.y;
			color++;
			Sleep(100);
			break;
		}

		}

	}
}

void gameover()
{
	//游戏结束释放资源
	_getch();
	closegraph();

}

int panduan(COLORREF cl, int x, int y)
{
	int a, b;
	//定义上下总个数
	int i = 0;
	a = x;
	b = y;
	/*x = xy.x;
	y = xy.y;*/
	//先检查上面的
	while (cl == getpixel(a - 3, b - 3 - 25))
	{
		i++;
		b -= 25;
	}
	//还原落子位置
	a = x;
	b = y;
	//再检查下面的
	while (cl == getpixel(a - 3, b - 3 + 25))
	{
		i++;
		b += 25;
	}
	//还原落子位置
	a = x;
	b = y;
	if (i >= 4)
	{
		return 1;      ///
	}

	//定义左右总个数
	int j = 0;
	a = x;
	b = y;
	//先检查左面的
	while (cl == getpixel(a - 3 - 25, b - 3))
	{
		j++;
		a -= 25;
	}
	//还原落子位置
	a = x;
	b = y;
	//再检查下面的
	while (cl == getpixel(a - 3 + 25, b - 3))
	{
		j++;
		a += 25;
	}
	//还原落子位置
	a = x;
	b = y;
	if (j >= 4)
	{
		return 1;
	}


	//定义'\'总个数
	int k = 0;
	a = x;
	b = y;
	//先检查上面的
	while (cl == getpixel(a - 3 - 25, b - 3 - 25))
	{
		k++;
		a -= 25;
		b -= 25;
	}
	//还原落子位置
	a = x;
	b = y;
	//再检查下面的
	while (cl == getpixel(a - 3 + 25, b - 3 + 25))
	{
		k++;
		a += 25;
		b += 25;
	}
	//还原落子位置
	a = x;
	b = y;
	if (k >= 4)
	{
		return 1;    //
	}


	//定义'/'总个数
	int l = 0;
	a = x;
	b = y;
	//先检查上面的
	while (cl == getpixel(a - 3 + 25, b - 3 - 25))
	{
		l++;
		a += 25;
		b -= 25;
	}
	//还原落子位置
	a = x;
	b = y;
	//再检查下面的
	while (cl == getpixel(a - 3 - 25, b - 3 + 25))
	{
		l++;
		a -= 25;
		b += 25;
	}
	//还原落子位置
	a = x;
	b = y;
	if (l >= 4)
	{
		return 1;
	}
	return 0;
}

void rjarrayqizhi()
{
	//  在白色棋子上显示黑色的顺序
	for (int i = 0; i <= color; i++)
	{
		if (i % 2 != 0)
		{
			setbkcolor(RGB(255, 255, 255));
			char arr[10];
			sprintf_s(arr, "%-2d", i + 1);
			settextcolor(RGB(0, 0, 0));
			settextstyle(15, 0, "宋体");
			outtextxy(rjarray[i].x - 7, rjarray[i].y - 7, arr);
		}
	}
	//恢复了背景颜色
	setbkcolor(RGB(249, 237, 225));
}

void rrarrayqizhi()
{
	//  在黑色棋子上显示白色的顺序
	for (int i = 0; i <= color; i++)
	{
		if (i % 2 == 0)
		{
			setbkcolor(RGB(0, 0, 0));
			char arr[10];
			sprintf_s(arr, "%-2d", i + 1);
			settextcolor(RGB(255, 255, 255));
			settextstyle(15, 0, "宋体");
			outtextxy(rrarray[i].x - 7, rrarray[i].y - 7, arr);
		}
	}
	//恢复了背景颜色
	setbkcolor(RGB(249, 237, 225));
}

int shpanduan(COLORREF cl, int x, int y, int q, int p)
{
	//用一个K来体现某一个方向的下一个是背景而不是有另外颜色的(活) K = 1 表示下一个为背景色 
	int K = 0;
	int a, b;
	//定义上下总个数
	int i = 0;
	a = x;
	b = y;
	//先检查上面的
	while (cl == getpixel(a - 3, b - 3 - 25))
	{
		i++;
		b -= 25;
	}
	//检查是否下一个是背景色 
	if (getpixel(a - 3, b - 3 - 25) == RGB(187, 161, 139) && b - 25 > 300)
	{
		K++;
	}
	//还原落子位置
	a = x;
	b = y;
	//再检查下面的
	while (cl == getpixel(a - 3, b - 3 + 25))
	{
		i++;
		b += 25;
	}
	//检查是否下一个是背景色 
	if (getpixel(a - 3, b - 3 + 25) == RGB(187, 161, 139) && b + 35 < 650)
	{
		K++;
	}
	//还原落子位置
	a = x;
	b = y;
	if (i == p - 1 && K == q)
	{
		return 1;
	}

	//重置K
	K = 0;
	//定义左右总个数
	int j = 0;
	a = x;
	b = y;
	//先检查左面的
	while (cl == getpixel(a - 3 - 25, b - 3))
	{
		j++;
		a -= 25;
	}
	//检查是否下一个是背景色 
	if (getpixel(a - 3 - 25, b - 3) == RGB(187, 161, 139) && a - 35 > 50)
	{
		K++;
	}
	//还原落子位置
	a = x;
	b = y;
	//再检查下面的
	while (cl == getpixel(a - 3 + 25, b - 3))
	{
		j++;
		a += 25;
	}
	//检查是否下一个是背景色 
	if (getpixel(a - 3 + 25, b - 3) == RGB(187, 161, 139) && a + 35 < 650)
	{
		K++;
	}
	//还原落子位置
	a = x;
	b = y;
	if (j == p - 1 && K == q)
	{
		return 1;
	}

	//重置K
	K = 0;
	//定义'\'总个数
	int k = 0;
	a = x;
	b = y;
	//先检查上面的
	while (cl == getpixel(a - 3 - 25, b - 3 - 25))
	{
		k++;
		a -= 25;
		b -= 25;
	}
	//检查是否下一个是背景色
	if (getpixel(a - 3 - 25, b - 3 - 25) == RGB(187, 161, 139) && a - 25 > 50 && b - 25 > 300)
	{
		K++;
	}
	//还原落子位置
	a = x;
	b = y;
	//再检查下面的
	while (cl == getpixel(a - 3 + 25, b - 3 + 25))
	{
		k++;
		a += 25;
		b += 25;
	}
	//检查是否下一个是背景色
	if (getpixel(a - 3 + 25, b - 3 + 25) == RGB(187, 161, 139) && a + 25 < 350 && b + 25 < 650)
	{
		K++;
	}
	//还原落子位置
	a = x;
	b = y;
	if (k == p - 1 && K == q)
	{
		return 1;
	}

	//重置K
	K = 0;
	int l = 0;
	a = x;
	b = y;
	//先检查上面的
	while (cl == getpixel(a - 3 + 25, b - 3 - 25))
	{
		l++;
		a += 25;
		b -= 25;
	}
	//检查是否下一个是背景色
	if (getpixel(a - 3 + 25, b - 3 - 25) == RGB(187, 161, 139) && a + 25 < 350 && b - 25 > 300)
	{
		K++;
	}
	//还原落子位置
	a = x;
	b = y;
	//再检查下面的
	while (cl == getpixel(a - 3 - 25, b - 3 + 25))
	{
		l++;
		a -= 25;
		b += 25;
	}
	//检查是否下一个是背景色
	if (getpixel(a - 3 - 25, b - 3 + 25) == RGB(187, 161, 139) && a - 25 > 50 && b + 25 < 650)
	{
		K++;
	}
	//还原落子位置
	a = x;
	b = y;
	if (l == p - 1 && K == q)
	{
		return 1;
	}
	return 0;
}

int scorepanduan(int x, int y)
{
	/*   主要思想
	电脑白子  自己黑子
	白子连成五子	+10000	阻碍黑子连成五子	+1000
	白子连成活四	+200	阻碍黑子连成活四	+100
	白子连成死四	+50	    阻碍黑子连成死四	+20
	白子连成活三	+30	    阻碍黑子连成活三	+10
	白子连成死三	+8	    阻碍黑子连成死三	+5
	白子连成活二	+2	    阻碍黑子连成活二	+1
	白子连成死二	+2	    阻碍黑子连成死二	+1
	白子连成活一	+1	    阻碍黑子连成活一	+0
	白子连成死一	+1	    阻碍黑子连成死一	+0
	—	*/
	int a = x;
	int b = y;
	//定义一个得分 
	int score = 0;

	//白子连成五子	+10000
	if (panduan(RGB(255, 255, 255), a, b))    //这个函数只是i的值与k的值不一样 所以可以加两个变量在函数里面 
	{
		score += 10000;
	}
	//阻碍黑子连成五子	+1000
	if (panduan(RGB(0, 0, 0), a, b))
	{
		score += 1000;
	}
	//白子连成活四	+200      
	if (shpanduan(RGB(255, 255, 255), a, b, 2, 4))
	{
		score += 200;
	}
	//阻碍黑子连成活四	+100
	if (shpanduan(RGB(0, 0, 0), a, b, 2, 4))
	{
		score += 100;
	}
	//白子连成死四	+50	
	if (shpanduan(RGB(255, 255, 255), a, b, 1, 4))
	{
		score += 50;
	}
	//阻碍黑子连成死四	+20
	if (shpanduan(RGB(0, 0, 0), a, b, 1, 4))
	{
		score += 20;
	}
	//白子连成活三	+30	
	if (shpanduan(RGB(255, 255, 255), a, b, 2, 3))
	{
		score += 30;
	}
	//阻碍黑子连成活三	+10
	if (shpanduan(RGB(0, 0, 0), a, b, 2, 3))
	{
		score += 10;
	}
	//白子连成死三	+8	
	if (shpanduan(RGB(255, 255, 255), a, b, 1, 3))
	{
		score += 8;
	}
	//阻碍黑子连成死三	+5
	if (shpanduan(RGB(0, 0, 0), a, b, 1, 3))
	{
		score += 5;
	}
	//白子连成活二	+2	
	if (shpanduan(RGB(255, 255, 255), a, b, 2, 2))
	{
		score += 2;
	}
	//阻碍黑子连成活二	+1
	if (shpanduan(RGB(0, 0, 0), a, b, 2, 2))
	{
		score += 1;
	}
	//白子连成死二	+2	
	if (shpanduan(RGB(255, 255, 255), a, b, 1, 2))
	{
		score += 2;
	}
	//阻碍黑子连成死二	+1
	if (shpanduan(RGB(0, 0, 0), a, b, 1, 2))
	{
		score += 1;
	}
	//白子连成活一	+1	
	if (shpanduan(RGB(255, 255, 255), a, b, 2, 1))
	{
		score += 1;
	}
	/* //阻碍黑子连成活一	+0
		if()
		{
			score += 0;
		}*/
		//白子连成死一	+1	
	if (shpanduan(RGB(255, 255, 255), a, b, 1, 1))
	{
		score += 1;
	}
	/* //阻碍黑子连成死一	+0
		if()
		{
			score += 0;
		}*/

	return score;
}

//******************************************************飞机大战部分********************************************************************

//----------------------------------------------------相关量定义------------------------------------------------------------------------
typedef struct Frame
{
	COORD position[2];
	int flag;
}Frame;

//------------------------------------------------------所有用到的函数定义以及功能说明--------------------------------------------------

//光标设置
void SetPos(COORD);//设置光标用
void SetPos(int, int);//设置光标用
void HideCursor();//表示不显示光标

//画图系列函数
void drawRow(int, int, int, char);//把第y行，[x1, x2] 之间的坐标填充为特定符号
void drawRow(COORD, COORD, char);//在a,b 纵坐标相同的前提下，把坐标[a, b] 之间填充为指定符号
void drawCol(int, int, int, char);//把第x列，[y1, y2] 之间的坐标填充为 ch
void drawCol(COORD, COORD, char);//在a, b 横坐标相同的前提下，把坐标 [a, b] 之间填充为 ch
void drawFrame(COORD, COORD, char, char);//左上角坐标、右下角坐标、用row填充行、用col填充列
void drawFrame(int, int, int, int, char, char);//左上角坐标、右下角坐标、用row填充行、用col填充列
void drawFrame(Frame, char, char);//左上角坐标、右下角坐标、用row填充行、用col填充列
void drawPlaying();//游戏界面画图
int drawMenu();//绘制初始引导面板，选择游戏模式

//随机生成函数
int random(int, int);//在[a, b)之间产生一个随机整数
COORD random(COORD, COORD);//在两个坐标包括的矩形框内随机产生一个坐标

//游戏功能函数
bool  judgeCoordInFrame(Frame, COORD);//中弹和坠毁判断

//--------------------------------------------------------函数功能实现----------------------------------------------------------------
void SetPos(COORD a)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(out, a);//移动命令行中光标的位置
}

void SetPos(int i, int j)
{
	COORD pos = { i, j };
	SetPos(pos);
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void drawRow(int y, int x1, int x2, char ch)
{
	SetPos(x1, y);
	for (int i = 0; i <= (x2 - x1); i++)
		cout << ch;
}

void drawRow(COORD a, COORD b, char ch)
{
	if (a.Y == b.Y)
		drawRow(a.Y, a.X, b.X, ch);
	else
	{
		SetPos(0, 25);
		cout << "error code 01：无法填充行，因为两个坐标的纵坐标(x)不相等";
		system("pause");
	}
}

void drawCol(int x, int y1, int y2, char ch)
{
	int y = y1;
	while (y != y2 + 1)
	{
		SetPos(x, y);
		cout << ch;
		y++;
	}
}

void drawCol(COORD a, COORD b, char ch)
{
	if (a.X == b.X)
		drawCol(a.X, a.Y, b.Y, ch);
	else
	{
		SetPos(0, 25);
		cout << "error code 02：无法填充列，因为两个坐标的横坐标(y)不相等";
		system("pause");
	}
}

void drawFrame(COORD a, COORD  b, char row, char col)
{
	drawRow(a.Y, a.X + 1, b.X - 1, row);
	drawRow(b.Y, a.X + 1, b.X - 1, row);
	drawCol(a.X, a.Y + 1, b.Y - 1, col);
	drawCol(b.X, a.Y + 1, b.Y - 1, col);
}

void drawFrame(int x1, int y1, int x2, int y2, char row, char col)
{
	COORD a = { x1, y1 };
	COORD b = { x2, y2 };
	drawFrame(a, b, row, col);
}

void drawFrame(Frame frame, char row, char col)
{
	COORD a = frame.position[0];
	COORD b = frame.position[1];
	drawFrame(a, b, row, col);
}

void drawPlaying()
{
	drawFrame(0, 0, 48, 24, '=', '|');//飞机大战地图
	drawFrame(49, 0, 79, 4, '-', '|');//结果显示
	drawFrame(49, 4, 79, 9, '-', '|');//得分称号显示区		
	drawFrame(49, 9, 79, 20, '-', '|');//操作提示区
	drawFrame(49, 20, 79, 24, '-', '|');//其他信息显示区
	SetPos(52, 5);
	cout << "击落战机：";
	SetPos(52, 6);
	cout << "当前得分：";
	SetPos(52, 7);
	cout << "当前段位：";
	SetPos(52, 10);
	cout << "操作方式：";
	SetPos(52, 12);
	cout << "  ↑,↓,←,→ 控制战机移动。";
	SetPos(52, 14);
	cout << "  p 控制暂停。";
	SetPos(52, 16);
	cout << "  e 退出游戏。";
	SetPos(52, 18);
	cout << "  s 攻击敌机。";
	SetPos(50, 21);
	cout << "右一提醒您：";
	SetPos(57, 22);
	cout << "适度游戏益脑";
	SetPos(57, 23);
	cout << "过度游戏伤身";
}

int drawMenu()
{
	SetPos(20, 12);
	cout << "欢迎来到右一杂货铺的飞机大战，祝游戏愉快！！！(*·▽·*)";
	drawRow(10, 0, 100, '-');
	drawRow(11, 0, 100, '-');
	SetPos(20, 14);
	cout << "操作提示： ";
	SetPos(22, 16);
	cout << "↑,↓,←,→ 控制战机移动。";
	SetPos(22, 18);
	cout << "p 控制暂停。";
	drawRow(25, 0, 100, '-');
	drawRow(26, 0, 100, '-');
	SetPos(22, 20);
	cout << "e 退出游戏。";
	SetPos(22, 22);
	cout << "s 开始游戏，并用于射击";

	while (1)
	{
		if (_kbhit())
		{
			char x = _getch();
			switch (x)
			{
			case 's':
			{
				return 2;
			}
			}
		}
	}
}

int random(int a, int b)
{
	int c = (rand() % (a - b)) + a;
	return c;
}

COORD random(COORD a, COORD b)
{
	int x = random(a.X, b.X);
	int y = random(a.Y, b.Y);
	COORD c = { x, y };
	return c;
}

bool  judgeCoordInFrame(Frame frame, COORD spot)
{
	if (spot.X >= frame.position[0].X)
		if (spot.X <= frame.position[1].X)
			if (spot.Y >= frame.position[0].Y)
				if (spot.Y <= frame.position[0].Y)
					return true;
	return false;
}

//--------------------------------------------------------类定义及类实现----------------------------------------------------------------
class Game
{
public:
	COORD position[10];
	COORD bullet[10];
	Frame enemy[8];
	int score;
	int rank;
	int rankf;
	string title;
	int flag_rank;

	Game();

	//初始化所有
	void initPlane();//飞机初始位置
	void initBullet();//子弹初始位置
	void initEnemy();//敌机初始位置

	void planeMove(char);//飞机移动
	void bulletMove();//子弹移动
	void enemyMove();//敌机移动

	//填充所有
	void drawPlane();//画飞机
	void drawPlaneToNull();//移动后原位置清掉
	void drawBullet();//画子弹
	void drawBulletToNull();//移动后子弹原位置清掉
	void drawEnemy();//画敌机
	void drawEnemyToNull();//移动后原位置清掉

	//填充其中一个
	void drawThisBulletToNull(COORD);//打敌机时
	void drawThisEnemyToNull(Frame);//被打时用

	void Pause();//中场暂停
	void Playing();//键盘操作
	void judgePlane();//坠毁判断
	void judgeEnemy();//被打判断

	void Shoot();//子弹射出

	void GameOver();//游戏结束
	void printScore();//分数打印
};

Game::Game()
{
	initPlane();
	initBullet();
	initEnemy();
	score = 0;
	rank = 45;
	rankf = 0;
	flag_rank = 0;
}

void Game::initPlane()
{
	COORD centren = { 30, 22 };
	position[0].X = position[5].X = position[7].X = position[9].X = centren.X;
	position[1].X = centren.X - 2;
	position[2].X = position[6].X = centren.X - 1;
	position[3].X = position[8].X = centren.X + 1;
	position[4].X = centren.X + 2;
	for (int i = 0; i <= 4; i++)
		position[i].Y = centren.Y;
	for (int i = 6; i <= 8; i++)
		position[i].Y = centren.Y + 1;
	position[5].Y = centren.Y - 1;
	position[9].Y = centren.Y - 2;
}

void Game::drawPlane()
{
	for (int i = 0; i < 9; i++)
	{
		SetPos(position[i]);
		if (i != 5)
			cout << "@";
		else if (i == 5)
			cout << "|";
	}
}

void Game::drawPlaneToNull()
{
	for (int i = 0; i < 9; i++)
	{
		SetPos(position[i]);
		cout << " ";
	}
}

void Game::initBullet()
{
	for (int i = 0; i < 10; i++)
		bullet[i].Y = 30;
}

void Game::drawBullet()
{
	for (int i = 0; i < 10; i++)
	{
		if (bullet[i].Y != 30)
		{
			SetPos(bullet[i]);
			cout << "*";
		}
	}
}

void Game::drawBulletToNull()
{
	for (int i = 0; i < 10; i++)
		if (bullet[i].Y != 30)
		{
			COORD pos = { bullet[i].X, bullet[i].Y + 1 };
			SetPos(pos);
			cout << " ";
		}
}

void Game::initEnemy()
{
	COORD a = { 1, 1 };
	COORD b = { 45, 15 };
	for (int i = 0; i < 8; i++)
	{
		enemy[i].position[0] = random(a, b);
		enemy[i].position[1].X = enemy[i].position[0].X + 3;
		enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
	}
}

void Game::drawEnemy()
{
	for (int i = 0; i < 8; i++)
		drawFrame(enemy[i].position[0], enemy[i].position[1], '$', '$');
}

void Game::drawEnemyToNull()
{
	for (int i = 0; i < 8; i++)
	{
		drawFrame(enemy[i].position[0], enemy[i].position[1], ' ', ' ');
	}
}

void Game::Pause()
{
	SetPos(61, 2);
	cout << "               ";
	SetPos(61, 2);
	cout << "暂停中...";
	char c = _getch();
	while (c != 'p')
		c = _getch();
	SetPos(61, 2);
	cout << "         ";
}

void Game::planeMove(char x)
{
	if (x == 75)
	{
		if (position[1].X != 1)
		{
			for (int i = 0; i <= 9; i++)
				position[i].X -= 2;
		}//
	}
	if (x == 80)
	{
		if (position[7].Y != 23)
		{
			for (int i = 0; i <= 9; i++)
				position[i].Y += 1;
		}
	}
	if (x == 77)
	{
		if (position[4].X != 47)
		{
			for (int i = 0; i <= 9; i++)
				position[i].X += 2;
		}//
	}
	if (x == 72)
	{
		if (position[5].Y != 3)
		{
			for (int i = 0; i <= 9; i++)
				position[i].Y -= 1;
		}
	}
	
}

void Game::bulletMove()
{
	for (int i = 0; i < 10; i++)
	{
		if (bullet[i].Y != 30)
		{
			bullet[i].Y -= 1;
			if (bullet[i].Y == 1)
			{
				COORD pos = { bullet[i].X, bullet[i].Y + 1 };
				drawThisBulletToNull(pos);
				bullet[i].Y = 30;
			}

		}
	}
}

void Game::enemyMove()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 2; j++)
			enemy[i].position[j].Y++;

		if (24 == enemy[i].position[1].Y)
		{
			COORD a = { 1, 1 };
			COORD b = { 45, 3 };
			enemy[i].position[0] = random(a, b);
			enemy[i].position[1].X = enemy[i].position[0].X + 3;
			enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
		}
	}
}

void Game::judgePlane()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 9; j++)
			if (judgeCoordInFrame(enemy[i], position[j]))
			{
				SetPos(62, 2);
				cout << "坠毁";
				drawFrame(enemy[i], '0', '0');
				Sleep(1000);
				GameOver();
				break;
			}
}

void Game::drawThisBulletToNull(COORD c)
{
	SetPos(c);
	cout << " ";
}

void Game::drawThisEnemyToNull(Frame f)
{
	drawFrame(f, ' ', ' ');
}

void Game::judgeEnemy()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 10; j++)
			if (judgeCoordInFrame(enemy[i], bullet[j]))
			{
				score += 5;
				drawThisEnemyToNull(enemy[i]);
				COORD a = { 1, 1 };
				COORD b = { 45, 3 };
				enemy[i].position[0] = random(a, b);
				enemy[i].position[1].X = enemy[i].position[0].X + 3;
				enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
				drawThisBulletToNull(bullet[j]);
				bullet[j].Y = 30;
			}
}

void Game::Shoot()
{
	for (int i = 0; i < 10; i++)
		if (bullet[i].Y == 30)
		{
			bullet[i].X = position[5].X;
			bullet[i].Y = position[5].Y - 1;
			break;
		}
}

void Game::printScore()
{
	if (score == 120 && flag_rank == 0)
	{
		rank -= 3;
		flag_rank = 1;
	}

	else if (score == 360 && flag_rank == 1)
	{
		rank -= 5;
		flag_rank = 2;
	}
	else if (score == 480 && flag_rank == 2)
	{
		rank -= 5;
		flag_rank = 3;
	}
	else if (score == 600 && flag_rank == 3)
	{
		rank -= 5;
		flag_rank = 4;
	}
	else if (score == 720 && flag_rank == 4)
	{
		rank -= 5;
		flag_rank = 5;
	}
	else if (score == 840 && flag_rank == 5)
	{
		rank -= 5;
		flag_rank = 6;
	}
	else if (score == 960 && flag_rank == 6)
	{
		rank -= 5;
		flag_rank = 7;
	}
	else if (score == 1080 && flag_rank == 7)
	{
		rank -= 5;
		flag_rank = 8;
	}
	int x = rank / 5;
	SetPos(65, 5);
	cout << score / 5;
	SetPos(65, 6);
	cout << score;

	if (rank != rankf)
	{
		SetPos(65, 7);
		if (x == 9)
			title = "青铜";
		else if (x == 8)
			title = "白银";
		else if (x == 7)
			title = "黄金";
		else if (x == 6)
			title = "铂金";
		else if (x == 5)
			title = "钻石";
		else if (x == 4)
			title = "星耀";
		else if (x == 3)
			title = "最强王者";
		else if (x == 2)
			title = "传奇王者";
		else if (x == 1)
			title = "荣耀王者";
		cout << title;
	}
	rankf = rank;
}

void Game::Playing()
{
	drawEnemy();
	drawPlane();

	int flag_bullet = 0;
	int flag_enemy = 0;

	while (1)
	{
		Sleep(8);
		if (_kbhit())
		{
			char x = _getch();
			if (72 == x ||80 == x || 77 == x || 75 == x)
			{
				drawPlaneToNull();
				planeMove(x);
				drawPlane();
				judgePlane();
				if (position[1].X <=1 || position[4].X >= 47)
				{
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "越界啦，快回去", "提示", MB_OK);
				}
			}
			else if ('p' == x)
				Pause();
			else if ('s' == x)
				Shoot();
			else if ('e' == x)
			{
				GameOver();
				break;
			}

		}
		/* 处理子弹 */
		if (0 == flag_bullet)
		{
			bulletMove();
			drawBulletToNull();
			drawBullet();
			judgeEnemy();
		}
		flag_bullet++;
		if (5 == flag_bullet)
			flag_bullet = 0;

		/* 处理敌人 */
		if (0 == flag_enemy)
		{
			drawEnemyToNull();
			enemyMove();
			drawEnemy();
			judgePlane();
		}
		flag_enemy++;
		if (flag_enemy >= rank)
			flag_enemy = 0;

		/* 输出得分 */
		printScore();
	}
}

void Game::GameOver()
{
	system("cls");
	COORD p1 = { 28,9 };
	COORD p2 = { 53,15 };
	drawFrame(p1, p2, '=', '|');
	SetPos(36, 12);
	string str = "Game Over!";
	for (int i = 0; i < str.size(); i++)
	{
		Sleep(80);
		cout << str[i];
	}
	Sleep(1000);
	system("cls");
	drawFrame(p1, p2, '=', '|');
	SetPos(31, 11);
	cout << "击落敌机：" << score / 5 << " 架";
	SetPos(31, 12);
	cout << "得　　分：" << score << " 分";
	SetPos(31, 13);
	cout << "获得称号：" << title;
	SetPos(30, 16);
	Sleep(1000);
	cout << "还想继续嘛？ 是（y）| 否（n）";
as:
	char x = _getch();
	if (x == 'n')
		exit(0);
	else if (x == 'y')
	{
		system("cls");
		Game game;
		int a = drawMenu();
		if (a == 2)
			game.rank = 45;
		system("cls");
		drawPlaying();
		game.Playing();
	}
	else goto as;
}

//******************************************************贪吃蛇部分********************************************************************
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
COORD coord;//光标定位

int m, n;//定义地图的长宽，蛇的坐标，长度，方向，食物的位置 
int direct[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };

void locate(int x, int y)
{
	coord.X = y;
	coord.Y = x;
	SetConsoleCursorPosition(hout, coord);
};

void hide()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
	SetConsoleCursorInfo(hout, &cursor_info);
}//隐藏光标

double random(double start, double end)
{
	return start + (end - start) * rand() / (RAND_MAX + 1.0);
}//随机数生成

struct node
{
	int x, y;
}snake[1000];

class Snake 
{
    public: 
		int snake_length, dir;
		node food;
		void print_wall();//输出墙
		void print_snake();//首次输出蛇，其中snake[0]代表头
		bool is_correct();//判断是否撞墙或者自撞 
		bool print_food();// 随机生成并输出食物位置 
		bool go_ahead();// 蛇的前进 
		void begin();//游戏开始
};

void Snake::print_wall()
{
	cout << " ";
	for (int i = 1; i <= n; i++)
		cout << "-";
	cout << endl;
	for (int j = 0; j <= m - 1; j++)
	{
		cout << "|";
		for (int i = 1; i <= n; i++) cout << " ";
		cout << "|" << endl;
	}
	cout << " ";
	for (int i = 1; i <= n; i++)
		cout << "-";
	drawFrame(45, 5, 72, 8, '-', '|');//得分称号显示区		
	drawFrame(45, 8, 72, 11, '-', '|');//操作提示区
	drawFrame(45, 11,72, 15, '-', '|');//其他信息显示区
	SetPos(46, 6);
	cout << "当前得分：";
	SetPos(46, 9);
	cout << "操作方式：";
	SetPos(46, 10);
	cout << "  ↑,↓,←,→ 控制小蛇移动。";
	SetPos(46, 12);
	cout << "右一提醒您：";
	SetPos(50, 13);
	cout << "适度游戏益脑";
	SetPos(50, 14);
	cout << "过度游戏伤身";
}

void Snake::print_snake()
{
	locate(snake[0].x, snake[0].y);
	cout << "@";
	for (int i = 1; i <= snake_length - 1; i++)
	{
		locate(snake[i].x, snake[i].y);
		cout << "*";
	}
}

bool Snake::is_correct()
{
	if (snake[0].x == 0 || snake[0].y == 0 || snake[0].x == m + 1 || snake[0].y == n + 1) return false;
	for (int i = 1; i <= snake_length - 1; i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) return false;
	}
	return true;
}

bool Snake::print_food()
{
	srand((unsigned)time(0));
	bool e;
	while (1)
	{
		e = true;
		int i = (int)random(0, m) + 1, j = (int)random(0, n) + 1;
		food.x = i; food.y = j;
		for (int k = 0; k <= snake_length - 1; k++)
		{
			if (snake[k].x == food.x && snake[k].y == food.y)
			{
				e = false; break;
			}
		}
		if (e) break;
	}
	locate(food.x, food.y);
	cout << "$";
	return true;
}

bool Snake::go_ahead()
{
	node temp;
	bool e = false;
	temp = snake[snake_length - 1];
	for (int i = snake_length - 1; i >= 1; i--)
		snake[i] = snake[i - 1];
	snake[0].x += direct[dir][0];
	snake[0].y += direct[dir][1];
	locate(snake[1].x, snake[1].y);
	cout << "*";
	/*** 吃到了食物 ***/
	if (snake[0].x == food.x && snake[0].y == food.y)
	{
		snake_length++;
		e = true;
		snake[snake_length - 1] = temp;
	}
	/*** 输出此时蛇状态 ***/
	if (!e)
	{
		locate(temp.x, temp.y);
		cout << " ";
	}
	else
		print_food();
	locate(snake[0].x, snake[0].y);
	cout << "@";
	/*** 如果自撞 ***/
	if (!is_correct())
	{
		system("cls");
		cout << "小蛇去天国啦" << endl << "这一世小蛇的长度为: " << snake_length << endl;
		cout << "客官你的善念积分为：" << snake_length * 5 - 25 << endl;
		cout << "想带小蛇开启轮回吗？ 是（y）| 否（n）";
	as:
		char x = _getch();
		if (x == 'n')
			exit(0);
		else if (x == 'y')
		{
			system("cls");
			cout << "再次选择你想要的难度吧" << endl;
			int hard;
			cin >> hard;
			if (hard <= 0 || hard > 100)
			{
				cout << "ERROR" << endl;
				system("pause");
				return 0;
			}
			/*** 数据全部初始化，包括蛇长，位置，方向 ***/
			Snake xiaoshe;
			xiaoshe.snake_length = 5;
			clock_t a, b;
			char ch;
			double hard_len;
			for (int i = 0; i <= 4; i++)
			{
				snake[i].x = 1;
				snake[i].y = 5 - i;
			}
			xiaoshe.dir = 3;
			/*** 输出初始地图，蛇与食物 ***/
			system("cls");
			hide();
			xiaoshe.begin();
			locate(m + 2, 0);
			cout << "Now length: ";
			/*** 开始游戏 ***/
			while (1)
			{
				/*** 难度随长度增加而提高 ***/
				hard_len = (double)xiaoshe.snake_length / (double)(m * n);
				/*** 调节时间，单位是ms ***/
				a = clock();
				while (1)
				{
					b = clock();
					if (b - a >= (int)(400 - 30 * hard) * (1 - sqrt(hard_len))) break;
				}

				/*** 接受键盘输入的上下左右，并以此改变方向 ***/
				if (_kbhit())
				{
					char ch = _getch();
					if (ch == -32)
					{
						ch = _getch();
						switch (ch)
						{
						case 72:
							if (xiaoshe.dir == 2 || xiaoshe.dir == 3)
								xiaoshe.dir = 0;
							break;
						case 80:
							if (xiaoshe.dir == 2 || xiaoshe.dir == 3)
								xiaoshe.dir = 1;
							break;
						case 75:
							if (xiaoshe.dir == 0 || xiaoshe.dir == 1)
								xiaoshe.dir = 2;
							break;
						case 77:
							if (xiaoshe.dir == 0 || xiaoshe.dir == 1)
								xiaoshe.dir = 3;
							break;

						}

					}
				}
				//前进 
				if (!xiaoshe.go_ahead()) break;
				//在最后输出此时长度 
				locate(m + 2, 12);
				cout << xiaoshe.snake_length;
				SetPos(48, 7);
				cout << xiaoshe.snake_length*5-25;
				
			}
		}
		else goto as;
		return false;
	}
	return true;
}

void Snake::begin()
{
	print_wall();
	print_snake();
	is_correct();
	print_food();
	go_ahead();
}

//******************************************************主函数功能实现********************************************************************
int main()
{
	cout << "Hello!!欢迎来到右一的游戏杂货铺！！！" << endl;
	cout << " " << endl;
	Sleep(1000);
	cout << "新店开张，多多关照！！！本杂货铺游戏持续更新，欢迎客官持续关注！！！" << endl;
	cout << " " << endl;
	Sleep(1000);
	cout << "小店当前有三款小游戏可供客官选择：" << endl;
	cout << "       如果客官想体验刺激的飞机大战，请选1；" << endl;
	cout << "       如果客官想挑战欢乐的人机对战五子棋，请选2。" << endl;
	cout << "       如果客官想助力贪吃小蛇的重生，积累善念，请选3。" << endl;
	cout << " " << endl;
	Sleep(1000);
	cout << "(*·▽·*)   祝游戏愉快！！！  (*·▽·*)" << endl;
	int xuanze;
	cin >> xuanze;
	if (xuanze == 1)
	{
		//游戏准备
		mciSendString("open BGM2.mp3 alias bg", NULL, 0, NULL);
		mciSendString("play bg repeat", NULL, 0, NULL);
		srand((int)time(0));	//随机种子
		HideCursor();	//隐藏光标

		Game game;
		int a = drawMenu();
		if (a == 2)
			game.rank = 45;
		system("cls");
		drawPlaying();
		game.Playing();
	}
	else if (xuanze == 2)
	{
		initgame();
		renjibegingame();
		gameover();
		system("pause");
		return 0;
	}
	else if (xuanze == 3)
	{
		mciSendString("open BGM3.mp3 alias bg", NULL, 0, NULL);
		mciSendString("play bg repeat", NULL, 0, NULL);

		cout << "------------------------------欢迎来到贪吃小蛇的重生轮回----------------------------" << endl;
		cout << "    游戏背景：" << endl;
		cout << "        这是一条被诅咒的小蛇，可怜的它被困在狭窄的四方墙之中。" << endl;
		cout << "        只有当它不断长大，以躯体冲破围墙，才能打破诅咒。" << endl;
		cout << "        每当它触碰到那四方围墙或者不幸触到自身，便会被打入天国，夺去性命。" << endl;
		cout << "        唯有好心人相助，才能拥有一次又一次改变命运的机会。" << endl;
		cout << " " << endl;
		cout << "    操作提示：" << endl;
		cout << "                用↑ ↓ ← → 控制小蛇的运动方向" << endl;
		cout << "                去吃$带小虫长身体吧！" << endl;
		cout << "                为了避免发生错位.建议将窗口调为最大哦." << endl;
		cout << "                客官可以自选难度嗷.请在1-10中输入1个数,1最简单,10则最难" << endl;
		cout << "------------------------------------------------------------------------------------" << endl;
		m = 25;
		n = 40;
		if (m < 10 || n < 10 || m>25 || n>40)
		{
			cout << "ERROR" << endl;
			system("pause");
			return 0;
		}
		int hard;
		cin >> hard;
		if (hard <= 0 || hard > 100)
		{
			cout << "ERROR" << endl;
			system("pause");
			return 0;
		}
		// 数据全部初始化，包括蛇长，位置，方向 
		Snake xiaoshe;
		xiaoshe.snake_length = 5;
		clock_t a, b;
		char ch;
		double hard_len;
		for (int i = 0; i <= 4; i++)
		{
			snake[i].x = 1;
			snake[i].y = 5 - i;
		}
		xiaoshe.dir = 3;
		// 输出初始地图，蛇与食物
		system("cls");
		hide();
		xiaoshe.begin();
		locate(m + 2, 0);
		cout << "Now length: ";
		//开始游戏
		while (1)
		{
			// 难度随长度增加而提高 
			hard_len = (double)xiaoshe.snake_length / (double)(m * n);
			//调节时间，单位是ms 
			a = clock();
			while (1)
			{
				b = clock();
				if (b - a >= (int)(400 - 30 * hard) * (1 - sqrt(hard_len))) break;
			}

			/*** 接受键盘输入的上下左右，并以此改变方向 ***/
			if (_kbhit())
			{
				char ch = _getch();
				if (ch == -32)
				{
					ch = _getch();
					switch (ch)
					{
					case 72:
						if (xiaoshe.dir == 2 || xiaoshe.dir == 3)
							xiaoshe.dir = 0;
						break;
					case 80:
						if (xiaoshe.dir == 2 || xiaoshe.dir == 3)
							xiaoshe.dir = 1;
						break;
					case 75:
						if (xiaoshe.dir == 0 || xiaoshe.dir == 1)
							xiaoshe.dir = 2;
						break;
					case 77:
						if (xiaoshe.dir == 0 || xiaoshe.dir == 1)
							xiaoshe.dir = 3;
						break;
					}
				}
			}
			//前进 
			if (!xiaoshe.go_ahead()) break;
			//在最后输出此时长度 
			locate(m + 2, 12);
			cout << xiaoshe.snake_length;
			SetPos(48, 7);
			cout << xiaoshe.snake_length * 5 - 25;
		}
		system("pause");
		return 0;
	}
	else
	{
		cout << "抱歉客官,右一这里好像还没有你要的游戏诶。" << endl;
		cout << " " << endl;
		Sleep(1000);
		cout << "客官可以看看小店已有的五子棋、飞机大战、贪吃蛇。或者告诉右一你想玩的游戏和联系方式，小店推出新品时及时通知您。 " << endl;
		cout << "(*·▽·*) " << endl;
		cout << " " << endl;
		Sleep(1000);
		char game[20];
		char phone[20];
		cout << "可以告诉右一你想玩的游戏吗？(*·▽·*)" << endl;
		cin >> game;
		cout << " " << endl;
		cout << "可以再告诉右一你的联系方式吗？(*·▽·*)" << endl;
		cin >> phone;
		cout << " " << endl;
		fstream fin;
		fin.open("C:\\Users\\lalayouyi\\Desktop\\jilu.txt", ios::ate | ios::out | ios::in);
        if (!fin)
		{
			HWND hWnd = GetHWnd();
			MessageBox(hWnd, "文件不存在", "提示", MB_OK);
			cout << "没事没事，右一记在脑子里啦，小店会及时更新哒，欢迎下次光临！！";
		}
		if (fin.is_open())
		{
			fin << game;
			fin << phone << endl;
			fin.close();
			cout << "右一记下啦，小店会及时更新哒，欢迎下次光临！！" << endl;
		}
		system("pause");
		return 0;
	}
}
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<Windows.h>
#include<time.h>

//界面函数
void Cursor(int x, int y);					//光标定位函数
void Draw();								//绘制初始化界面
void copy();								//绘制图标游戏名及版本号
void DrawWall();							//绘制墙壁
void runGame();								//游戏运行
void score();								//得分系统

//绘制与清除飞机函数
void DrawFly();								//绘制己方飞机
void CleanFly();							//清除己方飞机
void DrawDisFly();							//绘制敌方飞机
void CleanDisFly();							//清除敌方飞机
void DrawFlyBoo();							//绘制己方飞机炮弹
void DrawFlyBoo(int x, int y);				//重载绘制带位置参数的己方炮弹
void CleanFlyoo();							//清除己方炮弹
void CleanFlyBoo(int x,int y);				//重载清除带位置参数的己方炮弹
void DrawDisFlyBoo();						//绘制敌方飞机炮弹
void DrawDisFlyBoo(int x,int y);			//重载绘制带位置参数的敌方炮弹
void CleanDisFlyBoo();						//清除敌方炮弹
void CleanDisFlyBoo(int x,int y);			//重载清除带位置参数的敌方炮弹

//运行刷新函数
void ReDrawFly();							//重新绘制己方飞机
void ReDrawDisFly();						//重新绘制敌方飞机
void ReDrawFlyBoo();						//重新绘制己方炮弹
void ReDrawDisFlyBoo();						//重新绘制敌方炮弹
void DisFlyMove();							//控制敌方炮弹移动

//碰撞检测函数
void isFlyBoo();							//判断敌方飞机炮弹是否碰撞到己方飞机
void isDisFlyBoo();							//判断己方飞机炮弹是否碰撞到敌方飞机
void isUpFly();								//判断己方飞机是否升级
void isUpDisFly();							//判断敌方飞机是否升级

//结束函数
void OverDisFly();							//结束敌方飞机生命
void OverFly();								//结束己方飞机生命

//随机位置生成函数
void ProductDisFlyRander();					//生成敌方飞机横坐标随机位置
void MoveDisFlyRander();					//生成敌方飞机移动随机值

//界面控制
static const int Wall_x = 130;				//设置墙壁宽度值
static const int Wall_y = 40;				//设置墙壁高度值

//创建己方飞机位置结构体
struct _Fly{
	int x, y;
}; 

//创建敌方飞机位置结构体
struct Dis_Fly{
	int x, y;
};

//创建己方飞机炮弹结构体
struct FlyBoo{
	int x, y;
	struct FlyBoo *next;
};

//创建敌方飞机炮弹结构体
struct DisFlyBoo{
	int x, y;
	struct DisFlyBoo *next;
};

//设置刷新间隔时间SleepTime，	循环计数标志i，	当前循环计数标志i_now，	每次击中敌方飞机得分XXadd，	总得分XXscore，	当前击中敌方飞机得分XXscore_noe。
int SleepTime = 100, i = 0,i_now = 0, XXadd = 100, XXscore = 0, XXscore_now = 0;

//开辟指针型己方及敌方内存
_Fly* fly = (_Fly*)malloc(sizeof(_Fly));
Dis_Fly* disfly = (Dis_Fly*)malloc(sizeof(Dis_Fly));

//开辟指针型己方及敌方飞机炮弹内存
FlyBoo* flyboo = (FlyBoo*)malloc(sizeof(FlyBoo));
DisFlyBoo* disflyboo = (DisFlyBoo*)malloc(sizeof(DisFlyBoo));

//创建敌方飞机横坐标随机参数及飞机大小
struct DisFlyRander{
	int disflyRander_x;
	int disflybig;
}disflyRander;

//创建敌方飞机移动随机参数
struct DisFlyMoveRander{
	int moveRander;
}disflymoveRander;

void ProductDisFlyRander(){
	srand((unsigned)time(NULL));
	disflyRander.disflyRander_x = (rand() % (Wall_x - 3 * disflyRander.disflybig) + disflyRander.disflybig) / 2 * 2 + 1;
	//创建横坐标并使创建的横坐标在不可超出墙壁并处在偶数坐标格中（使用/2*2使得处在偶数格中）
}

void MoveDisFlyRander(){
	srand((unsigned)time(NULL));
	disflymoveRander.moveRander = rand() % 3 - 1;
	//使生成-1,0,1的随机数控制敌方飞机向左，静止或向右运动
}

void Cursor(int x, int y){

	COORD cursor;										//实例化结构体
	HANDLE hOutput;										//实例化句柄
	cursor.X = x;										//传入横坐标参数
	cursor.Y = y;										//传入纵坐标参数
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);			//获取输出框界面句柄
	SetConsoleCursorPosition(hOutput, cursor);			//定位坐标位置
}

void copy(){
	Cursor(0,Wall_y / 2 - 6 );
	printf("	  **********************                           *                       *\n");
	printf("                              *                               *                 *\n");
	printf("                            *                                    *            *\n");
	printf("                          *                                        *        *\n");
	printf("                        *                                            *    *\n");
	printf("                      *                                                **\n");
	printf("                    *                                                  **\n");
	printf("                  *                                                    **\n");
	printf("                *                                                      **\n");
	printf("              *                                                        **\n");
	printf("            *                                                          **\n");
	printf("         ***********************                                       **             。          。          。\n");
	Cursor(Wall_x-20, Wall_y );
	printf("打飞机   V 1.0");
	Cursor(0, Wall_y);		//使光标处在左下角
}


void DrawWall(){
	for (int i = 1; i < Wall_x; i+=2){
		Cursor(i, 0);		//定位上方墙壁
		printf("┰");		//绘制上方墙壁
		Cursor(i, Wall_y);	//定位下方墙壁
		printf("┸");		//绘制下方墙壁
	}
}
void DrawFly(){
	Cursor(fly->x, fly->y);				//定位己方待绘制飞机位置
	printf("◢■◣");					//绘制己方飞机
}

void CleanFly(){
	Cursor(fly->x, fly->y);				//定位己方待清除飞机位置
	printf("     ");					//清除己方飞机
}

void DrawDisFly(){
	Cursor(disfly->x, disfly->y);		//定位敌方待绘制飞机位置
	printf("◥□◤");					//绘制敌方飞机
}

void CleanDisFly(){
	Cursor(disfly->x, disfly->y);		//定位敌方待清除飞机位置
	printf("     ");					//清除敌方飞机
}

void DrawFlyBoo(){
	Cursor(flyboo->x, flyboo->y);		//定位己方待绘制炮弹位置
	printf("★");						//绘制己方炮弹
}

void DrawFlyBoo(int x, int y){
	Cursor(x, y);						//定位己方待绘制炮弹位置
	printf("★");						//绘制己方炮弹
}

void CleanFlyBoo(){
	Cursor(flyboo->x, flyboo->y);		//定位己方待清除炮弹位置
	printf("  ");						//清除己方炮弹
}
void CleanFlyBoo(int x,int y){
	Cursor(x, y);						//定位己方待清除炮弹位置
	printf("  ");						//清除己方炮弹
}

void DrawDisFlyBoo(){
	Cursor(disflyboo->x, disflyboo->y);	//定位敌方待绘制炮弹位置
	printf("☆");						//绘制敌方炮弹
}

void DrawDisFlyBoo(int x, int y){
	Cursor(x, y);						//定位敌方待绘制炮弹位置
	printf("☆");						//绘制敌方炮弹
}

void CleanDisFlyBoo(){
	Cursor(disflyboo->x, disflyboo->y);	//定位敌方待清除炮弹位置
	printf("  ");						//清除敌方炮弹
}

void CleanDisFlyBoo(int x,int y){
	Cursor(x, y);						//定位敌方待清除炮弹位置
	printf("  ");						//清除敌方炮弹
}


void ReDrawFly(){
	if (GetAsyncKeyState(VK_LEFT) && fly->x > 4){	//当用户键入左键
		CleanFly();									//清除原有己方飞机
		fly->x -= 2;								//向左移动单位网格长度
		DrawFly();									//重新绘制己方飞机
	}
	else if (GetAsyncKeyState(VK_RIGHT) && fly->x < Wall_x - 4){	//当用户键入右键
		CleanFly();													//清除原有己方飞机
		fly->x += 2;												//向右移动单位网格长度
		DrawFly();													//重新绘制己方飞机
	}
	else{
		DrawFly();						//若用户没有键入按键，进行刷新己方飞机，防止得分刷新覆盖
	}
}

void ReDrawDisFly(){
	DrawDisFly();						//当敌方飞机死亡时，进行重新绘制

}

void ReDrawFlyBoo(){
	FlyBoo* temp_flyboo = (FlyBoo*)malloc(sizeof(FlyBoo));			//开辟指针型己方临时飞机炮弹内存
	temp_flyboo->next = NULL;										//将其链接接口赋值为空

//清除原有己方飞机炮弹
	temp_flyboo = flyboo;								//通过临时变量进行下游查找遍历
	do{													//至少进行一次遍历，普适第一枚己方炮弹
		CleanFlyBoo(temp_flyboo->x,temp_flyboo->y);		//清除己方飞机炮弹
		temp_flyboo = temp_flyboo->next;				//指向下游节点
	} while (temp_flyboo != NULL);						//遍历至最后一枚己方炮弹

//将每个炮弹进行向上移动
	temp_flyboo = flyboo;								//通过临时变量进行下游查找遍历
	do{													//至少进行一次遍历，普适第一枚己方炮弹
		temp_flyboo->y -= 1;							//将每一枚己方炮弹纵坐标减一
		temp_flyboo = temp_flyboo->next;				//指向下游节点
	} while (temp_flyboo != NULL);						//遍历至最后一枚己方炮弹

//依照当前己方飞机链接新炮弹
	FlyBoo* next_flyboo = (FlyBoo*)malloc(sizeof(FlyBoo));	//开辟指针型递进己方飞机炮弹内存
	next_flyboo->x = fly->x + 2;							//通过当前己方飞机横坐标位置定位己方炮弹横坐标位置
	next_flyboo->y = fly->y - 1;							//通过当前己方飞机纵坐标位置定位己方炮弹纵坐标位置
	next_flyboo->next = flyboo;								//将炮弹首部控制在前端
	flyboo = next_flyboo;

//将己方炮弹全部重绘
	temp_flyboo = flyboo;								//通过临时变量进行下游查找遍历
	do{													//至少进行一次遍历，普适第一枚己方炮弹
		DrawFlyBoo(temp_flyboo->x, temp_flyboo->y);		//绘制己方炮弹
		temp_flyboo = temp_flyboo->next;				//指向下游节点
	} while (temp_flyboo != NULL);						//遍历至最后一枚己方炮弹

//判断己方炮弹是否超出画布
	if (i > 5){															//避免己方初始炮弹无二阶next指向
		temp_flyboo = flyboo;											//通过临时变量进行下游查找遍历
		while (temp_flyboo->next->next != NULL)							//查找至炮弹第后三个，避免无法指定释放内存炮弹上游指向为空
			temp_flyboo = temp_flyboo->next;							//指向下游节点

		if (temp_flyboo->next->y <= 1){									//当己方飞机炮弹末端纵坐标小于墙面时
			CleanFlyBoo(temp_flyboo->next->x, temp_flyboo->next->y);	//清除己方飞机炮弹
			free(temp_flyboo->next);									//释放己方飞机炮弹末端内存空间
			temp_flyboo->next = NULL;									//使被释放空间上游节点指向为空
		}
	}
	i++;																//运行循环次数递增
}

void ReDrawDisFlyBoo(){
	DisFlyBoo* temp_disflyboo = (DisFlyBoo*)malloc(sizeof(DisFlyBoo));	//开辟指针型敌方临时飞机炮弹内存
	temp_disflyboo->next = NULL;										//将其链接接口赋值为空

//清除原有敌方飞机炮弹
	temp_disflyboo = disflyboo;											//通过临时变量进行下游查找遍历
	do{																	//至少进行一次遍历，普适第一枚敌方炮弹
		CleanDisFlyBoo(temp_disflyboo->x,temp_disflyboo->y);			//清除敌方飞机炮弹
		temp_disflyboo = temp_disflyboo->next;							//指向下游节点
	} while (temp_disflyboo != NULL);									//遍历至最后一枚敌方炮弹

//将每个炮弹进行向上移动
	temp_disflyboo = disflyboo;											//通过临时变量进行下游查找遍历
	do{																	//至少进行一次遍历，普适第一枚敌方炮弹
		temp_disflyboo->y += 1;											//将每一枚敌方炮弹纵坐标减一
		temp_disflyboo = temp_disflyboo->next;							//指向下游节点
	} while (temp_disflyboo != NULL);									//遍历至最后一枚敌方炮弹

//依照当前敌方飞机链接新炮弹
	DisFlyBoo* next_disflyboo = (DisFlyBoo*)malloc(sizeof(DisFlyBoo));	//开辟指针型递进敌方飞机炮弹内存
	next_disflyboo->x = disfly->x + 2;									//通过当前敌方飞机横坐标位置定位敌方炮弹横坐标位置
	next_disflyboo->y = disfly->y + 1;									//通过当前敌方飞机纵坐标位置定位敌方炮弹纵坐标位置
	next_disflyboo->next = disflyboo;									//将炮弹首部控制在前端
	disflyboo = next_disflyboo;

//将敌方炮弹全部重绘
	temp_disflyboo = disflyboo;											//通过临时变量进行下游查找遍历
	do{																	//至少进行一次遍历，普适第一枚敌方炮弹
		DrawDisFlyBoo(temp_disflyboo->x, temp_disflyboo->y);			//绘制敌方炮弹
		temp_disflyboo = temp_disflyboo->next;							//指向下游节点
	} while (temp_disflyboo != NULL);									//遍历至最后一枚敌方炮弹

//判断敌方炮弹是否超出画布
	if (i > 5){															//避免敌方初始炮弹无二阶next指向
		temp_disflyboo = disflyboo;										//通过临时变量进行下游查找遍历
		while (temp_disflyboo->next->next != NULL)						//查找至炮弹第后三个，避免无法指定释放内存炮弹上游指向为空
			temp_disflyboo = temp_disflyboo->next;						//指向下游节点

		if (temp_disflyboo->next->y >= Wall_y - 1){						//当敌方飞机炮弹末端纵坐标小于墙面时
			CleanDisFlyBoo(temp_disflyboo->next->x, temp_disflyboo->next->y);	//清除敌方飞机炮弹
			free(temp_disflyboo->next);									//释放敌方飞机炮弹末端内存空间
			temp_disflyboo->next = NULL;								//使被释放空间上游节点指向为空
		}
	}
}

void DisFlyMove(){
	MoveDisFlyRander();													//调用随机函数生成运动随机数
	if (!(disfly->x > Wall_x - 5 && disflymoveRander.moveRander > 0) && !(disfly->x < 5 && disflymoveRander.moveRander < 0)){
																		//控股敌方飞机运动至墙面边界时无法持续滑动
		CleanDisFly();													//将原有敌方飞机清除
		disfly->x += disflymoveRander.moveRander;						//向指定随机方向进行运动
		DrawDisFly();													//重新绘制敌方飞机
	}
}

void score(){
	if ((XXscore != XXscore_now)||(i<=(i_now+3))){						//当飞机的得分更新或显示次数小于三次时
		Cursor(0, Wall_y - i + i_now);									//将光标指向墙面左下角并向上递减输出
		printf("%d  !!!  %d  !!!  %d", XXscore, XXscore, XXscore);		//输出当前得分
	}
}

void isFlyBoo(){
	DisFlyBoo* temp_disflyboo = (DisFlyBoo*)malloc(sizeof(DisFlyBoo));	//开辟指针型临时敌方飞机炮弹内存
	temp_disflyboo = disflyboo;											//通过临时变量进行下游遍历查找
	do{
		if ((temp_disflyboo->x >= fly->x) && (temp_disflyboo->x <= fly->x + 6) && (temp_disflyboo->y == fly->y - 1))	//当敌方飞机炮弹位于己方飞机平面坐标时
			OverFly();													//执行结束己方飞机生命函数
		temp_disflyboo = temp_disflyboo->next;							//指向下游节点
	} while (temp_disflyboo != NULL);									//遍历至最后一枚敌方炮弹

}

void isDisFlyBoo(){
	FlyBoo* temp_flyboo = (FlyBoo*)malloc(sizeof(FlyBoo));				//开辟指针型临时己方飞机炮弹内存
	temp_flyboo = flyboo;												//通过临时变量进行下游遍历查找
	do{
		if ((temp_flyboo->x >= disfly->x) && (temp_flyboo->x <= disfly->x + 6) && (temp_flyboo->y == disfly->y + 1))	//当己方飞机炮弹位于敌方飞机平面坐标时
			OverDisFly();												//执行结束敌方飞机生命函数
		temp_flyboo = temp_flyboo->next;								//指向下游节点
	} while (temp_flyboo != NULL);										//遍历至最后一枚己方炮弹
}

void isUpFly(){
	//待创建
}

void isUpDisFly(){
	//待创建
}

void OverDisFly(){						
	CleanDisFly();										//清除敌方飞机
	ProductDisFlyRander();								//生成敌方飞机待生成位置随机函数
	disfly->x = disflyRander.disflyRander_x + 2;		//指定敌方飞机所在横坐标
	disfly->y = 2;										//指定敌方飞机所在纵坐标
	DrawDisFly();										//绘制敌方飞机
	XXscore += XXadd;									//更新当前得分
	i_now = i;											//更新当前循环计数器
	XXscore_now = XXscore;								//更新当前分数计数器

}


void Draw(){
	system("mode con cols=132 lines=42");					//改变DOS大小
	copy();													//绘制商标信息
	system("pause");										//使用户按任意键继续
	system("cls");											//清屏，防止欠覆盖留白
	DrawWall();												//绘制墙体

//初始化己方飞机
		fly->x = Wall_x / 2;								//依照墙体指定己方飞机横坐标
		fly->y = Wall_y - 1;								//依照墙体指定己方飞机纵坐标
	DrawFly();												//绘制己方飞机

//初始化敌方飞机
		ProductDisFlyRander();								//生成敌方飞机待生成位置随机函数
		disfly->x = disflyRander.disflyRander_x + 2;		//依照随机数指定敌方飞机横坐标
		disfly->y = 2;										//依照随机数指定敌方飞机纵坐标
	DrawDisFly();											//绘制敌方飞机

//绘制己方飞机炮弹
		disflyRander.disflybig = 6;							//指定敌方飞机初始大小
		flyboo->x = fly->x + 2;								//依照当前己方飞机横坐标指定己方飞机炮弹横坐标
		flyboo->y = fly->y - 1;								//依照当前己方飞机纵坐标指定己方飞机炮弹纵坐标
		flyboo->next = NULL;								//将初始飞机炮弹下游指向位置赋值为空
	DrawFlyBoo();											//绘制己方飞机炮弹

//绘制敌方飞机炮弹
	disflyboo->x = disfly->x + 2;							//依照当前敌方飞机横坐标指定敌方飞机炮弹横坐标
		disflyboo->y = disfly->y + 1;						//依照当前敌方飞机纵坐标指定敌方飞机炮弹纵坐标
		disflyboo->next = NULL;								//将初始飞机炮弹下游指向位置赋值为空
	DrawDisFlyBoo();										//绘制敌方飞机炮弹
}

void runGame(){
	while (1){												//进行游戏循环
		isFlyBoo();
		isDisFlyBoo();
		ReDrawFly();
		ReDrawDisFly();
		ReDrawFlyBoo();
		ReDrawDisFlyBoo();
		DisFlyMove();
		isUpFly();
		isUpDisFly();
		score();
		Sleep(SleepTime);
	}
}

void OverFly(){
	Sleep(600);												//给予失败证据提示
	system("cls");											//清屏
	for (int i = 0; i <= 500; i++)							//进行失败效果制作
		printf("Game Over!     ");							//输出游戏失败
	system("pause");										//等待用户按任意键退出游戏
	exit(0);												//结束游戏关闭程序
}

int main(){

	Draw();													//进行初始化绘制
	runGame();												//进行游戏运行
}
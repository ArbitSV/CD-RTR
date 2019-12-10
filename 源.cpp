//0.0.1
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<set>
#include<cstdlib>
#include<functional>
#include<cmath>
#include<ctime>
#include<string>
#include<cstring>
#include<iostream>
#include<Windows.h>
#include<vector>
#include<iterator>
using namespace std;
using std::cout;
using std::printf;
using std::cin;
using std::fflush;
using std::system;

char move_type;
int obj_pos;

struct player
{
private:
	int hp;//血量
	int axisX;//所在行编号
	int axisY;//所在列编号
	int hp_f;//初始血量
	char face;//朝向：w上；s下；a左；d右
	string name;
	//bool alive = true;
public:
	player() :hp(100), hp_f(100), axisX(-1), axisY(-1), face('0'), name("yet set")
	{}
	void set_pos(int x, int y)
	{
		this->axisX = x;
		this->axisY = y;
	}
	void set_name(string nm)
	{
		this->name = nm;
	}
	void set_face(char f)
	{
		this->face = f;
	}
	string get_name()
	{
		return this->name;
	}
	int get_x()
	{
		return this->axisX;
	}
	int get_y()
	{
		return this->axisY;
	}
	char get_face()
	{
		return this->face;
	}
};

vector<player> player_list;
player _NULL;

struct block
{
	int axisX;
	int axisY;
	char status; 
	player pl;
	//status描述状态：e表示空；p表示有玩家；r表示为废墟；o表示有障碍物；d表示被人走过；@表示有人死在这里
	block() :axisX(0), axisY(0), status('e'), pl(_NULL)
	{}
};

block map[8][11];
//map[i][j]表示第i行第j列的格子

bool judge_dead()
{
	if (map[player_list[0].get_x()][player_list[0].get_y()].status == '@')
	{
		cout << player_list[0].get_name() << " died!" << endl;
		cout << player_list[1].get_name() << " wins this game!" << endl;
		return true;
	}
	else if (map[player_list[1].get_x()][player_list[1].get_y()].status == '@')
	{
		cout << player_list[1].get_name() << " died!" << endl;
		cout << player_list[0].get_name() << " wins this game!" << endl;
		return true;
	}
	return false;
}
	
int convert_char_int(char input)
{
	set<char> judge = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	if (judge.find(input) == judge.cend())
	{
		return static_cast<int>(input);
	}
	else
	{
		return (atoi(&input));
	}
}

//此函数包含更换map中status参数以及对player中的pos参数的更换
void set_pl_pos(player&pl, int&x, int&y)
{
	map[pl.get_x()][pl.get_y()].status = 'd';
	map[pl.get_x()][pl.get_y()].pl = _NULL;
	pl.set_pos(x, y);
	map[x][y].status = 'p';
	map[x][y].pl = pl;
}

player&find_pl(int x, int y)
{
	if (map[x][y].status != 'p')
	{
		return _NULL;
	}
	else
	{
		for (auto it = player_list.begin();it != player_list.end();++it)
		{
			if ((*it).get_x() == x && (*it).get_y() == y)
			{
				return (*it);
			}
		}
	}
	return _NULL;
}

void print_map()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 11; ++j)
		{
			switch (map[i][j].status)
			{
			case 'e':
				printf("0");
				break;
			case 'p':
				printf("%s",find_pl(i,j).get_name().c_str());
				break;
			case 'r':
				printf("*");
				break;
			case 'o':
				printf("o");
				break;
			case 'd':
				printf("x");
				break;
			case '@':
				printf("@");
				break;
			default:
				break;
			}
			printf("\t");
			if (j == 10)
			{
				printf("\n\n");
			}
		}
	}
}

void move(player&pl)
{
	int x, y;
	x = pl.get_x();
	y = pl.get_y();
	switch (move_type)
	{
	case 'w':
		--x;
		if (x == -1)
			x = 7;
		set_pl_pos(pl, x, y);
		pl.set_face('w');
		break;
	case 'a':
		--y;
		if (y == -1)
			y = 10;
		set_pl_pos(pl, x, y);
		pl.set_face('a');
		break;
	case 's':
		++x;
		if (x == 8)
			x = 0;
		set_pl_pos(pl, x, y);
		pl.set_face('s');
		break;
	case 'd':
		++y;
		if (y == 11)
			y = 0;
		set_pl_pos(pl, x, y);
		pl.set_face('d');
		break;
	default:
		break;
	}
}

void convert_obj(int x, int y)
{
	map[x][y].status = 'o';
}

void place_obj(player pl)
{
	switch (obj_pos)
	{
	case 1:
		convert_obj(pl.get_x() - 1, pl.get_y() - 1);
		break;
	case 2:
		convert_obj(pl.get_x() - 1, pl.get_y());
		break;
	case 3:
		convert_obj(pl.get_x() - 1, pl.get_y() + 1);
		break;
	case 4:
		convert_obj(pl.get_x(), pl.get_y() - 1);
		break;
	case 6:
		convert_obj(pl.get_x(), pl.get_y() + 1);
		break;
	case 7:
		convert_obj(pl.get_x() + 1, pl.get_y() - 1);
		break;
	case 8:
		convert_obj(pl.get_x() + 1, pl.get_y());
		break;
	case 9:
		convert_obj(pl.get_x() + 1, pl.get_y() + 1);
		break;
	default:
		break;
	}
}

//unfinished
void shoot(player&pl)
{
	switch (pl.get_face())
	{
	case 'w':
		for (int i = pl.get_x() - 1;i >= 0;--i)
		{
			if (map[i][pl.get_y()].status == 'o')
			{
				map[i][pl.get_y()].status = 'r';
				break;
			}
			else if (map[i][pl.get_y()].status == 'p')
			{
				map[i][pl.get_y()].status = '@';
				break;
			}
		}
		break;
	case 's':
		for (int i = pl.get_x() + 1; i <= 7; ++i)
		{
			if (map[i][pl.get_y()].status == 'o')
			{
				map[i][pl.get_y()].status = 'r';
				break;
			}
			else if (map[i][pl.get_y()].status == 'p')
			{
				map[i][pl.get_y()].status = '@';
				break;
			}
		}
		break;
	case 'a':
		for (int i = pl.get_y() - 1; i >= 0; --i)
		{
			if (map[pl.get_x()][i].status == 'o')
			{
				map[pl.get_x()][i].status = 'r';
				break;
			}
			else if (map[pl.get_x()][i].status == 'p')
			{
				map[pl.get_x()][i].status = '@';
				break;
			}
		}
		break;
	case 'd':
		for (int i = pl.get_y() + 1; i <= 10; ++i)
		{
			if (map[pl.get_x()][i].status == 'o')
			{
				map[pl.get_x()][i].status = 'r';
				break;
			}
			else if (map[pl.get_x()][i].status == 'p')
			{
				map[pl.get_x()][i].status = '@';
				break;
			}
		}
		break;
	default:
		break;
	}
}

//移动函数：type {s：开枪|o：放置障碍物|m(w/a/s/d)：移动}
void operate(player&pl,char&type)
{
	switch (type)
	{
	case 's':
		shoot(pl);
		break;
	case 'o':
		place_obj(pl);
		break;
	case 'm':
		move(pl);
		break;
	}
}

//返回false表示合法；true表示非法
bool operate_judge(player pl,char type,char move_type,int obj_pos)
{
	int tempx, tempy;
	tempx = pl.get_x();
	tempy = pl.get_y();
	bool jge = true;
	//cout << pl.get_x() << " " << pl.get_y() << endl;
	switch (type)
	{
	case 'm':
		switch (move_type)
		{
		case 'w':
			--tempx;
			if (tempx == -1)
				tempx = 7;
			break;
		case 'a':
			--tempy;
			if (tempy == -1)
				tempy = 10;
			break;
		case 's':
			++tempx;
			if (tempx == 8)
				tempx = 0;
			break;
		case 'd':
			++tempy;
			if (tempy == 11)
				tempy = 0;
			break;
		default:
			break;
		}
		//cout << map[tempx][tempy].status << endl;
		//cout << tempx << " " << tempy << endl;
		if (map[tempx][tempy].status == 'e')
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 'o':
		//bool final_judge = true;//true表示合法，false表示非法
		switch (obj_pos)
		{
		case 1:
			--tempx;
			--tempy;
			break;
		case 2:
			--tempx;
			break;
		case 3:
			--tempx;
			++tempy;
			break;
		case 4:
			--tempy;
			break;
		case 6:
			++tempy;
			break;
		case 7:
			++tempx;
			--tempy;
			break;
		case 8:
			++tempx;
			break;
		case 9:
			++tempx;
			++tempy;
			break;
		default:
			break;
		}
		if (tempx < 0 || tempy < 0)
		{
			return true;
		}
		else
		{
			if (map[tempx][tempy].status == 'e')
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		//experimental
		break;
	case 's':
		//true表示非法；false表示合法
		switch (pl.get_face())
		{
		case 'w':
			for (int i = pl.get_x() - 1; i >= 0; --i)
			{
				if (map[i][pl.get_y()].status == 'p' || map[i][pl.get_y()].status == 'o')
				{
					jge = false;
					break;
				}
			}
			break;
		case 's':
			for (int i = pl.get_x() + 1; i <= 7; ++i)
			{
				if (map[i][pl.get_y()].status == 'p' || map[i][pl.get_y()].status == 'o')
				{
					jge = false;
					break;
				}
			}
			break;
		case 'a':
			for (int i = pl.get_y() - 1; i >= 0; --i)
			{
				if (map[pl.get_x()][i].status == 'p' || map[pl.get_x()][i].status == 'o')
				{
					jge = false;
					break;
				}
			}
			break;
		case 'd':
			for (int i = pl.get_y() + 1; i <= 10; --i)
			{
				if (map[pl.get_x()][i].status == 'p' || map[pl.get_x()][i].status == 'o')
				{
					jge = false;
					break;
				}
			}
			break;
		}
		return jge;
		break;
	default:
		return true;
		break;
	}
}

//true代表被困，false代表安全
bool judge_trap(player pl)
{
	for (int i = 1; i <= 9; ++i)
	{
		if (i == 5)
			continue;
		if (operate_judge(pl, 'o', ::move_type, i) == false)
			return false;
	}
	set<char> mt = {'w','a','s','d'};
	for (auto c : mt)
	{
		if (operate_judge(pl, 'm', c, ::obj_pos) == false)
			return false;
	}
	if (operate_judge(pl, 's', ::move_type, ::obj_pos) == false)
		return false;
	return true;
}

int main()
{
	//初始化
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 11; ++j)
		{
			map[i][j].axisX = i;
			map[i][j].axisY = j;
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		player A;
		player_list.push_back(A);
	}
	player_list[0].set_pos(0, 0);
	map[0][0].pl = player_list[0];
	map[0][0].status = 'p';
	player_list[0].set_name("A");
	player_list[1].set_pos(7, 10);
	map[7][10].status = 'p';
	player_list[1].set_name("B");
	map[7][10].pl = player_list[1];
	print_map();
	//初始化结束
	//开始游戏循环
	char input;
	while (1)
	{
		//A
		printf("%s's turn\n", player_list[0].get_name().c_str());
		printf("Help:\n");
		cout << "'m' for move; 'o' for place an obstacle; 's' for shoot" << endl;
		fflush(stdin);
		cin >> input;
		switch (input)
		{
		case 'm':
			cout << "select move type:" << endl;
			//cout << "" << endl;
			fflush(stdin);
			cin >> move_type;
			break;
		case 'o':
			char pt;
			cout << "input built pos:" << endl;
			fflush(stdin);
			cin >> pt;
			obj_pos = convert_char_int(pt);
			break;
		case 's':
			break;
		default:
			break;
		}
		while (operate_judge(player_list[0], input, move_type, obj_pos))
		{
			cout << "wrong input!" << endl;
			if (input == 's')
			{
				cout << "no target!" << endl;
			}
			fflush(stdin);
			cin >> input; 
			switch (input)
			{
		    case 'm':
				cout << "select move type:" << endl;
				fflush(stdin);
				cin >> move_type; 
				break;
			case 'o':
				char pt;
				cout << "input built pos:" << endl;
				fflush(stdin);
				cin >> pt;
				obj_pos = convert_char_int(pt);
				break;
			case 's':
				break;
			default:
				break;
			}
		}
		operate(player_list[0], input);
		//cout << find_pl(player_list[0].get_x(), player_list[0].get_y()).get_name() << endl;
		system("cls");
		print_map();
		//judge dead
		if (judge_trap(player_list[0]) == true)
			map[player_list[0].get_x()][player_list[0].get_y()].status = '@';
		if (judge_dead() == true)
		{
			break;
		}
		//B
		printf("%s's turn\n", player_list[1].get_name().c_str());
		printf("Help:\n");
		fflush(stdin);
		cin >> input;
		switch (input)
		{
		case 'm':
			cout << "select move type:" << endl;
			fflush(stdin);
			cin >> move_type;
			break;
		case 'o':
			char pt;
			cout << "input built pos:" << endl;
			fflush(stdin);
			cin >> pt;
			obj_pos = convert_char_int(pt);
			break;
		case 's':
			break;
		default:
			break;
		}
		while (operate_judge(player_list[1], input, move_type, obj_pos))
		{
			cout << "wrong input!" << endl;
			if (input == 's')
			{
				cout << "no target!" << endl;
			}
			fflush(stdin);
			cin >> input;
			switch (input)
			{
			case 'm':
				cout << "select move type:" << endl;
				fflush(stdin);
				cin >> move_type;
				break;
			case 'o':
				char pt;
				cout << "input built pos:" << endl;
				fflush(stdin);
				cin >> pt;
				obj_pos = convert_char_int(pt);
				break;
			case 's':
				break;
			default:
				break;
			}
		}
		operate(player_list[1], input);
		system("cls");
		//cout << find_pl(player_list[1].get_x(), player_list[1].get_y()).get_name() << endl;
		print_map();
		//judge dead
		if (judge_trap(player_list[1]) == true)
			map[player_list[1].get_x()][player_list[1].get_y()].status = '@';
		if (judge_dead() == true)
		{
			break;
		}
	}
	printf("ended\n");
	system("pause");
}













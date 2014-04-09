#pragma once

#include <iostream>
#include <conio.h>
#include <cstdlib>  // srand, rand
#include <ctime> // time, clock
#include <windows.h>
#include <iomanip>
#include <list>

using namespace std;


void zero()
{
	HANDLE hCon = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD z = {0, 0};
	::SetConsoleCursorPosition(hCon, z);
}

struct Point
{
	int x;
	int y;
	Point(const int& _x = -1, const int& _y = -1)
		: x(_x), y(_y)
	{}
};

struct mover
{
	enum dir_id
	{
		LEFT,
		UP,
		RIGHT,
		DOWN,
	};

	mover(const char& c, const Point& pt, const int& dir = LEFT)
		: mark(c), pos(pt), direction(dir)
	{}

	Point follow(const Point& newpt)
	{
		Point prev = pos;

		pos = newpt;

		return prev;
	}
	char mark;
	Point pos;
	int direction;
};

class game
{
	enum dir_id
	{
		LEFT,
		UP,
		RIGHT,
		DOWN,
	};

public :
	game(const int& w = 30, const int& h = 10)
		: width(w), height(h)
		, blank('-'), border('#'), meat('$')
		, need_to_update(true), need_to_draw(true), meat_exist(false)
		, snake('@', Point(width/2, height/2)), myMeat()
		, meat_count(0), bordersize(1), mp_index(0)
		, board(NULL), meat_position(NULL)
		, speed(1.f), update_dt(0)
		, direction(LEFT)
	{
		srand((unsigned)time(NULL));

		board = new char[width*height];
		meat_position = new int[width*height];
	}
	~game()
	{
		if (board)
			delete board;
		if (meat_position)
			delete meat_position;

		// 동적 생성된 mover 객체 메모리 해제.
		std::list<mover*>::iterator it;
		for (it = body.begin(); it != body.end(); it++)
		{
			delete (*it);
		}
	}

	void Setup()
	{
		// 1. 데이터 초기화...
		// 1-1. initialize board data.
		for (int i = 0; i < width*height; i++)
		{
			board[i] = blank;
		}
		// 1-2. initialize meat position index.
		for (int i = 0; i < width*height; i++)
		{
			meat_position[i] = i;
		}
		// 1-3. shuffle meat position index.
		for (int i = width*height-1; i > 0; i--)
		{
			int index = rand()%i;

			int tmp = meat_position[i];
			meat_position[i] = meat_position[index];
			meat_position[index] = tmp;
		}
	}
	bool Input()
	{
		if (_kbhit())
		{
			int key = _getch();
			if (key == 0x1B)
			{
				return false;
			}
			else if (key == 'a' || key == 'A')
			{
				snake.direction = LEFT;
			}
			else if (key == 'd' || key == 'D')
			{
				snake.direction = RIGHT;
			}
			else if (key == 'w' || key == 'W')
			{
				snake.direction = UP;
			}
			else if (key == 's' || key == 'S')
			{
				snake.direction = DOWN;
			}
			else if (key == '+')
			{
				speed += 0.1f;
			}

			need_to_update = true;
		}

		return true;
	}
	void Update(clock_t tick)
	{
		if (speed*update_dt/1000 >= 1)
		{
			int count = speed*update_dt/1000; // 칸수

			update_dt = update_dt - (count/speed)*1000;

			for (int i = 0; i < count; i++)
			{
				// 4-1. 먹었는지 안먹었는지 확인
				if (snake.pos.x == myMeat.x && snake.pos.y == myMeat.y)
				{
					meat_exist = false;
					meat_count++;

					mover* pBody = new mover('*', Point(-1,-1));
					body.push_back(pBody);

					speed = speed + 0.1f;
				}

				if (snake.direction == LEFT)
				{
					if (snake.pos.x == bordersize)
						snake.direction = UP;
					else
					{
						Point prev = snake.follow(Point(snake.pos.x-1, snake.pos.y));
						std::list<mover*>::iterator it;
						for (it = body.begin(); it != body.end(); it++)
						{
							prev = (*it)->follow(prev);
						}
					}
				}
				else if (snake.direction == UP)
				{
					if (snake.pos.y == bordersize)
						snake.direction = RIGHT;
					else
					{
						Point prev = snake.follow(Point(snake.pos.x, snake.pos.y-1));
						std::list<mover*>::iterator it;
						for (it = body.begin(); it != body.end(); it++)
						{
							prev = (*it)->follow(prev);
						}
					}
				}
				else if (snake.direction == RIGHT)
				{
					if (snake.pos.x + 1 == width - bordersize)
						snake.direction = DOWN;
					else
					{
						Point prev = snake.follow(Point(snake.pos.x+1, snake.pos.y));
						std::list<mover*>::iterator it;
						for (it = body.begin(); it != body.end(); it++)
						{
							prev = (*it)->follow(prev);
						}
					}
				}
				else if (snake.direction == DOWN)
				{
					if (snake.pos.y + 1 == height - bordersize)
						snake.direction = LEFT;
					else
					{
						Point prev = snake.follow(Point(snake.pos.x, snake.pos.y+1));
						std::list<mover*>::iterator it;
						for (it = body.begin(); it != body.end(); it++)
						{
							prev = (*it)->follow(prev);
						}
					}
				}

				// todo update.
				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width; x++)
					{
						if (x < bordersize || y < bordersize ||
							x >= width-bordersize || y >= height-bordersize)
							board[x + y*width] = border;
						else if (snake.pos.x == x && snake.pos.y == y)
							board[x + y*width] = snake.mark;
						else if (myMeat.x == x && myMeat.y == y)
							board[x + y*width] = meat;
						else
							board[x + y*width] = blank;

						std::list<mover*>::iterator it;
						for (it = body.begin(); it != body.end(); it++)
						{
							if ((*it)->pos.x == x && (*it)->pos.y == y)
							{
								board[x + y*width] = (*it)->mark;
							}
						}
					}
				}

				// 4-3. 먹이가 존재하지 않으면 먹이를 생성하자.
				if (!meat_exist)
				{
					// 만들고...
					while (true)
					{
						if (board[meat_position[mp_index]] != blank)
						{
							mp_index++;
							if (mp_index >= width*height)
								mp_index=0;
						}
						else
						{
							myMeat.x = meat_position[mp_index]%width;
							myMeat.y = meat_position[mp_index]/width;
							board[meat_position[mp_index]] = meat;
							break;
						}
					}

					meat_exist = true;
				}
			}

			need_to_draw = true;
			need_to_update = false;
		}

		update_dt += tick;
	}
	void Draw()
	{
		if (need_to_draw)
		{
			zero();

			cout << "Catch Meat~ by anonymous..." << endl;
			// todo draw..
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					cout << board[x + y*width];
				}
				cout << endl;
			}
			cout << "current eaten count : " << meat_count << endl;
			cout << "current speed : " << setw(4) << speed << endl;

			need_to_draw = false;
		}
	}

private :
	// 0. 사용할 변수 및 상수를 선언
	const int width;
	const int height;
	const char blank;
	const char border;
	const char meat;
	bool need_to_update;
	bool need_to_draw;
	bool meat_exist;
	Point myMeat;
	int meat_count;
	int bordersize;

	mover snake;
	std::list<mover*> body;

	char *board;
	int *meat_position;
	int mp_index;

	//////////////////////////
	// 자동 움직임...
	float speed;			// 1칸/s
	clock_t update_dt;

	int direction; // left(0), top(1), right(2), bottom(3)
};
